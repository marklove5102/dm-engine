#include "BTEngine.h"
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <sstream>

// 安全字符串转整数，失败返回默认值
static int SafeStoi(const std::wstring& s, int defVal = 0)
{
    try { return std::stoi(s); }
    catch (...) { return defVal; }
}

// ============================================================================
// XML 解析器实现
// ============================================================================

std::shared_ptr<BTNode> XMLParser::ParseFile(const std::wstring& filePath)
{
    std::wstring xml = ReadFileGBK(filePath);
    if (xml.empty()) return nullptr;
    return ParseWString(xml);
}

std::shared_ptr<BTNode> XMLParser::ParseString(const std::string& xmlContent)
{
    // 自适应编码：先尝试 UTF-8，失败则回退到系统默认代码页（简体中文 Windows 为 GBK）
    int wlen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, xmlContent.c_str(), (int)xmlContent.size(), nullptr, 0);
    UINT codePage = CP_UTF8;
    if (wlen <= 0)
    {
        codePage = CP_ACP;
        wlen = MultiByteToWideChar(CP_ACP, 0, xmlContent.c_str(), (int)xmlContent.size(), nullptr, 0);
    }
    if (wlen <= 0) return nullptr;
    std::wstring xml(wlen, L'\0');
    MultiByteToWideChar(codePage, 0, xmlContent.c_str(), (int)xmlContent.size(), &xml[0], wlen);
    return ParseWString(xml);
}

std::shared_ptr<BTNode> XMLParser::ParseWString(const std::wstring& xml)
{
    // 跳过 XML 声明
    size_t pos = xml.find(L"<BehaviorTree");
    if (pos == std::wstring::npos) return nullptr;
    pos = xml.find(L'>', pos);
    if (pos == std::wstring::npos) return nullptr;
    pos++;

    // 跳过注释和空白，找到第一个真正的根节点
    while (pos < xml.length())
    {
        SkipWhitespace(xml, pos);
        if (pos >= xml.length()) break;
        if (xml.substr(pos, 4) == L"<!--")
        {
            pos = xml.find(L"-->", pos);
            if (pos == std::wstring::npos) break;
            pos += 3;
            continue;
        }
        if (xml[pos] == L'<')
        {
            XMLElement rootElem;
            pos = ParseElement(xml, pos, rootElem);
            if (!rootElem.tagName.empty())
                return BuildTree(rootElem, nullptr, 0);
        }
        break;
    }
    return nullptr;
}

std::wstring XMLParser::Trim(const std::wstring& s)
{
    size_t start = 0, end = s.length();
    while (start < end && (s[start] == L' ' || s[start] == L'\t' || s[start] == L'\r' || s[start] == L'\n'))
        start++;
    while (end > start && (s[end - 1] == L' ' || s[end - 1] == L'\t' || s[end - 1] == L'\r' || s[end - 1] == L'\n'))
        end--;
    return s.substr(start, end - start);
}

void XMLParser::SkipWhitespace(const std::wstring& xml, size_t& pos)
{
    while (pos < xml.length() && (xml[pos] == L' ' || xml[pos] == L'\t' || xml[pos] == L'\r' || xml[pos] == L'\n'))
        pos++;
}

std::wstring XMLParser::ReadName(const std::wstring& xml, size_t& pos)
{
    size_t start = pos;
    while (pos < xml.length() && (iswalnum(xml[pos]) || xml[pos] == L'_' || xml[pos] == L'-' || xml[pos] == L':'))
        pos++;
    return xml.substr(start, pos - start);
}

std::wstring XMLParser::ReadUntil(const std::wstring& xml, size_t& pos, wchar_t delim)
{
    size_t start = pos;
    while (pos < xml.length() && xml[pos] != delim)
        pos++;
    std::wstring result = xml.substr(start, pos - start);
    if (pos < xml.length()) pos++; // 跳过分隔符
    return result;
}

std::wstring XMLParser::DecodeEntities(const std::wstring& text)
{
    std::wstring result = text;
    size_t pos;
    while ((pos = result.find(L"&lt;")) != std::wstring::npos) result.replace(pos, 4, L"<");
    while ((pos = result.find(L"&gt;")) != std::wstring::npos) result.replace(pos, 4, L">");
    while ((pos = result.find(L"&amp;")) != std::wstring::npos) result.replace(pos, 5, L"&");
    while ((pos = result.find(L"&quot;")) != std::wstring::npos) result.replace(pos, 6, L"\"");
    while ((pos = result.find(L"&apos;")) != std::wstring::npos) result.replace(pos, 6, L"'");
    return result;
}

size_t XMLParser::ParseElement(const std::wstring& xml, size_t pos, XMLElement& outElem)
{
    if (pos >= xml.length() || xml[pos] != L'<')
        return pos;

    pos++; // 跳过 '<'

    // 检查是否是结束标签
    if (pos < xml.length() && xml[pos] == L'/')
    {
        return pos; // 返回结束标签位置
    }

    // 跳过注释 <!-- ... --> 和声明 <!DOCTYPE ...> / <![CDATA[ ... ]]>
    if (pos < xml.length() && xml[pos] == L'!')
    {
        if (pos + 3 <= xml.length() && xml[pos + 1] == L'-' && xml[pos + 2] == L'-')
        {
            // XML 注释 <!-- ... -->
            pos = xml.find(L"-->", pos + 3);
            if (pos != std::wstring::npos)
                return pos + 3; // 跳过 -->
        }
        else
        {
            // 其他声明 <!DOCTYPE ...> 或 <![CDATA[ ... ]]>
            pos = xml.find(L'>', pos);
            if (pos != std::wstring::npos)
                return pos + 1;
        }
        return pos; // 未找到闭合，返回当前位置
    }

    // 读取标签名
    outElem.tagName = ReadName(xml, pos);

    // 读取属性
    while (pos < xml.length())
    {
        SkipWhitespace(xml, pos);
        if (pos >= xml.length()) break;

        if (xml[pos] == L'>')
        {
            pos++; // 跳过 '>'
            // 读取文本内容
            size_t textStart = pos;
            while (pos < xml.length() && xml[pos] != L'<')
                pos++;
            outElem.text = Trim(DecodeEntities(xml.substr(textStart, pos - textStart)));

            // 解析子元素
            while (pos < xml.length())
            {
                if (pos + 1 < xml.length() && xml[pos] == L'<' && xml[pos + 1] == L'/')
                {
                    // 结束标签
                    pos += 2;
                    SkipWhitespace(xml, pos);
                    std::wstring endTag = ReadName(xml, pos);
                    SkipWhitespace(xml, pos);
                    if (pos < xml.length() && xml[pos] == L'>') pos++;
                    break;
                }
                else if (xml[pos] == L'<')
                {
                    XMLElement child;
                    size_t newPos = ParseElement(xml, pos, child);
                    if (newPos == pos) break; // 防止死循环
                    if (!child.tagName.empty())
                        outElem.children.push_back(std::move(child));
                    pos = newPos;
                }
                else
                {
                    pos++;
                }
            }
            return pos;
        }
        else if (xml[pos] == L'/' && pos + 1 < xml.length() && xml[pos + 1] == L'>')
        {
            pos += 2; // 跳过 '/>'
            return pos;
        }
        else
        {
            // 属性名
            std::wstring attrName = ReadName(xml, pos);
            if (attrName.empty())
            {
                // 死循环防护：遇到非法字符时跳过它继续
                pos++;
                continue;
            }
            SkipWhitespace(xml, pos);
            if (pos < xml.length() && xml[pos] == L'=')
            {
                pos++; // 跳过 '='
                SkipWhitespace(xml, pos);
                if (pos < xml.length() && (xml[pos] == L'"' || xml[pos] == L'\''))
                {
                    wchar_t quote = xml[pos];
                    pos++;
                    std::wstring attrValue = ReadUntil(xml, pos, quote);
                    outElem.attributes[attrName] = DecodeEntities(attrValue);
                }
            }
        }
    }
    return pos;
}

std::shared_ptr<BTNode> XMLParser::BuildTree(const XMLElement& elem, BTNode* parent, int depth)
{
    auto node = std::make_shared<BTNode>();
    node->id = std::to_wstring(depth) + L"_" + elem.tagName + L"_" + std::to_wstring((size_t)node.get());
    node->type = ParseNodeType(elem.tagName);
    node->conditionType = ParseConditionType(elem.tagName);
    node->actionType = ParseActionType(elem.tagName);
    node->parent = parent;
    node->depth = depth;

    // 读取 name 属性
    auto nameIt = elem.attributes.find(L"name");
    if (nameIt != elem.attributes.end())
        node->name = nameIt->second;
    else
        node->name = elem.tagName;

    // 读取其他属性作为参数（排除 name）
    for (auto& attr : elem.attributes)
    {
        if (attr.first != L"name")
            node->params[attr.first] = attr.second;
    }

    // 递归构建子节点
    for (auto& child : elem.children)
    {
        auto childNode = BuildTree(child, node.get(), depth + 1);
        if (childNode)
            node->children.push_back(childNode);
    }

    return node;
}

std::wstring XMLParser::ReadFileGBK(const std::wstring& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return L"";

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(size, '\0');
    file.read(&buffer[0], size);
    file.close();

    // 编码自动检测：BOM 头
    UINT codePage = 936; // 默认 GBK
    const char* rawData = buffer.c_str();
    size_t offset = 0;

    if (size >= 3 && (unsigned char)rawData[0] == 0xEF && (unsigned char)rawData[1] == 0xBB && (unsigned char)rawData[2] == 0xBF)
    {
        codePage = CP_UTF8;   // UTF-8 BOM
        offset = 3;
    }
    else if (size >= 2 && (unsigned char)rawData[0] == 0xFF && (unsigned char)rawData[1] == 0xFE)
    {
        codePage = 1200;      // UTF-16 LE
        offset = 2;
    }
    else if (size >= 2 && (unsigned char)rawData[0] == 0xFE && (unsigned char)rawData[1] == 0xFF)
    {
        codePage = 1201;      // UTF-16 BE
        offset = 2;
    }
    else if (size > 0)
    {
        // 无 BOM：试探 UTF-8 有效性
        int testLen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, rawData, (int)size, nullptr, 0);
        if (testLen > 0)
            codePage = CP_UTF8;
    }

    int wlen = MultiByteToWideChar(codePage, 0, rawData + offset, (int)(size - offset), nullptr, 0);
    std::wstring result(wlen, L'\0');
    MultiByteToWideChar(codePage, 0, rawData + offset, (int)(size - offset), &result[0], wlen);
    return result;
}

// ============================================================================
// 行为树引擎实现
// ============================================================================

BTEngine::BTEngine()
    : m_rng(std::random_device{}())
{
}

void BTEngine::Reset()
{
    m_logs.clear();
    m_memPositions.clear();
    m_decoratorState.clear();
    m_stepCount = 0;
    m_execOrder = 0;
    // 注意：frameTime 与 m_decoratorState 的时间状态由 ClearTimeState() 管理
    // Reset 只清日志/记忆位置/装饰器状态/计数，不再重置 frameTime
    // 这样自动播放时 AdvanceFrame 累加的时间不会被 ExecuteFull→Reset 清零
}

void BTEngine::ResetNodeStates(std::shared_ptr<BTNode> node)
{
    if (!node) return;
    node->lastResult = BTResult::IDLE;
    node->isActive = false;
    node->execOrder = -1;
    for (auto& child : node->children)
        ResetNodeStates(child);
}

BTResult BTEngine::ExecuteNode(std::shared_ptr<BTNode> node)
{
    if (!node) return BTResult::FAILURE;

    node->isActive = true;
    node->execOrder = m_execOrder++;

    BTResult result;
    switch (node->type)
    {
    case BTNodeType::SEQUENCE:          result = ExecuteSequence(node); break;
    case BTNodeType::SELECTOR:          result = ExecuteSelector(node); break;
    case BTNodeType::PARALLEL:          result = ExecuteParallel(node); break;
    case BTNodeType::RANDOM:            result = ExecuteRandom(node); break;
    case BTNodeType::PROBABILITY:       result = ExecuteProbability(node); break;
    case BTNodeType::MEM_SEQUENCE:      result = ExecuteMemSequence(node); break;
    case BTNodeType::MEM_SELECTOR:      result = ExecuteMemSelector(node); break;
    case BTNodeType::INVERTER:          result = ExecuteInverter(node); break;
    case BTNodeType::DECORATOR_REPEAT:  result = ExecuteDecoratorRepeat(node); break;
    case BTNodeType::DECORATOR_TIMEOUT: result = ExecuteDecoratorTimeout(node); break;
    case BTNodeType::DECORATOR_COOLDOWN:result = ExecuteDecoratorCooldown(node); break;
    case BTNodeType::DECORATOR_PERIODIC:result = ExecuteDecoratorPeriodic(node); break;
    case BTNodeType::SUCCEEDER:         result = ExecuteSucceeder(node); break;
    case BTNodeType::FAILER:            result = ExecuteFailer(node); break;
    case BTNodeType::CONDITION:         result = ExecuteCondition(node); break;
    case BTNodeType::ACTION:            result = ExecuteAction(node); break;
    default: result = BTResult::FAILURE;
    }

    node->lastResult = result;
    node->isActive = false;
    AddLog(node->id, node->name, result, node->depth, node->type);
    return result;
}

void BTEngine::ClearTimeState()
{
    m_decoratorState.clear();
    m_ctx.frameTime = 0;
}

void BTEngine::ExecuteFull(std::shared_ptr<BTNode> root)
{
    // 完整执行(单步):清空全部状态含时间,独立运行一次
    ClearTimeState();
    Reset();
    ResetNodeStates(root);
    ExecuteNode(root);
}

void BTEngine::ExecuteStep(std::shared_ptr<BTNode> root)
{
    // 自动播放:保留跨tick时间状态(m_decoratorState/frameTime)
    // 使 ActionRest(duration)/ActionRevive(delay)/DecoratorCooldown(ms) 等
    // 时间相关节点能跨 tick 累计计时,产生可见的 RUNNING→SUCCESS 演变
    m_logs.clear();
    m_stepCount = 0;
    m_execOrder = 0;
    // 不清 m_decoratorState,不重置 frameTime(由 AdvanceFrame 累加)
    // 不清 m_memPositions(MemSequence/MemSelector 跨tick记忆)
    ResetNodeStates(root);
    ExecuteNode(root);
}

void BTEngine::AddLog(const std::wstring& nodeId, const std::wstring& nodeName, BTResult result, int depth, BTNodeType type)
{
    LogEntry entry;
    entry.nodeId = nodeId;
    entry.nodeName = nodeName;
    entry.result = result;
    entry.depth = depth;
    entry.type = type;
    m_logs.push_back(entry);
    m_stepCount++;
}

// ============================================================================
// 复合节点 - 语义与 GameServer BotBehaviorComposite.cpp 完全对齐
// ============================================================================
BTResult BTEngine::ExecuteSequence(std::shared_ptr<BTNode> node)
{
	for (auto& child : node->children)
	{
		BTResult r = ExecuteNode(child);
		if (r != BTResult::SUCCESS)
			return r;   // FAILURE/RUNNING 立即返回
	}
	return BTResult::SUCCESS;
}

BTResult BTEngine::ExecuteSelector(std::shared_ptr<BTNode> node)
{
	for (auto& child : node->children)
	{
		BTResult r = ExecuteNode(child);
		if (r == BTResult::SUCCESS)
			return BTResult::SUCCESS;
		// FAILURE 则继续尝试下一个子节点
	}
	return BTResult::FAILURE;
}

BTResult BTEngine::ExecuteParallel(std::shared_ptr<BTNode> node)
{
	int nSuccess = 0, nFailure = 0;
	for (auto& child : node->children)
	{
		BTResult r = ExecuteNode(child);
		if (r == BTResult::SUCCESS) nSuccess++;
		else if (r == BTResult::FAILURE) nFailure++;
	}
	// 全部成功才成功；任一失败即失败；否则 RUNNING
	if (nSuccess == (int)node->children.size()) return BTResult::SUCCESS;
	if (nFailure > 0) return BTResult::FAILURE;
	return BTResult::RUNNING;
}

// GameServer CBTRandom::Execute 始终返回 SUCCESS（仅执行随机子节点的副作用）
BTResult BTEngine::ExecuteRandom(std::shared_ptr<BTNode> node)
{
	if (node->children.empty()) return BTResult::SUCCESS;
	int idx = (int)(m_rng() % node->children.size());
	ExecuteNode(node->children[idx]);
	return BTResult::SUCCESS;
}

// GameServer CBTProbability 仅持有单个子节点 m_pChild
BTResult BTEngine::ExecuteProbability(std::shared_ptr<BTNode> node)
{
	auto it = node->params.find(L"chance");
	int chance = it != node->params.end() ? SafeStoi(it->second, 50) : 50;
	if (chance < 0) chance = 0;
	if (chance > 100) chance = 100;
	if ((int)(m_rng() % 100) < chance)
	{
		if (!node->children.empty())
			return ExecuteNode(node->children[0]);   // 仅执行第一个子节点
		return BTResult::SUCCESS;
	}
	return BTResult::FAILURE;
}

// GameServer CBTMemSequence: FAILURE 记住当前索引（下 tick 重试该节点），RUNNING 直接返回
BTResult BTEngine::ExecuteMemSequence(std::shared_ptr<BTNode> node)
{
	size_t startIdx = 0;
	auto it = m_memPositions.find(node->id);
	if (it != m_memPositions.end()) startIdx = it->second;

	for (size_t i = startIdx; i < node->children.size(); i++)
	{
		BTResult r = ExecuteNode(node->children[i]);
		if (r == BTResult::FAILURE)
		{
			m_memPositions[node->id] = i;   // 记住失败位置，下 tick 从此重试
			return BTResult::FAILURE;
		}
		if (r == BTResult::RUNNING)
			return BTResult::RUNNING;       // 保留 startIdx，下 tick 继续
	}
	m_memPositions.erase(node->id);
	return BTResult::SUCCESS;
}

// GameServer CBTMemSelector: SUCCESS 记住当前索引（下 tick 从该节点继续），RUNNING 直接返回
BTResult BTEngine::ExecuteMemSelector(std::shared_ptr<BTNode> node)
{
	size_t startIdx = 0;
	auto it = m_memPositions.find(node->id);
	if (it != m_memPositions.end()) startIdx = it->second;

	for (size_t i = startIdx; i < node->children.size(); i++)
	{
		BTResult r = ExecuteNode(node->children[i]);
		if (r == BTResult::SUCCESS)
		{
			m_memPositions[node->id] = i;   // 记住成功位置
			return BTResult::SUCCESS;
		}
		if (r == BTResult::RUNNING)
			return BTResult::RUNNING;
	}
	m_memPositions.erase(node->id);
	return BTResult::FAILURE;
}

// ============================================================================
// 装饰节点 - 语义与 GameServer BotBehaviorDecorator.cpp 对齐
// 时间相关装饰器使用 m_ctx.frameTime 模拟游戏帧时间
// ============================================================================
BTResult BTEngine::ExecuteInverter(std::shared_ptr<BTNode> node)
{
	if (node->children.empty()) return BTResult::FAILURE;
	BTResult r = ExecuteNode(node->children[0]);
	switch (r)
	{
	case BTResult::SUCCESS: return BTResult::FAILURE;
	case BTResult::FAILURE: return BTResult::SUCCESS;
	default: return BTResult::RUNNING;
	}
}

// GameServer CBTDecoratorRepeat: 重复执行直到子节点返回 FAILURE，失败时返回 SUCCESS
// m_nMaxRepeat=0 表示无限（调试器用硬限制 100 防死循环，与服务端一致）
BTResult BTEngine::ExecuteDecoratorRepeat(std::shared_ptr<BTNode> node)
{
	if (node->children.empty()) return BTResult::FAILURE;
	auto it = node->params.find(L"count");
	int count = it != node->params.end() ? SafeStoi(it->second, 0) : 0;
	const int HARD_LIMIT = 100;
	int nEffectiveMax = (count > 0) ? count : HARD_LIMIT;

	for (int i = 0; i < nEffectiveMax; i++)
	{
		BTResult r = ExecuteNode(node->children[0]);
		if (r == BTResult::FAILURE)
			return BTResult::SUCCESS;   // 重复直到失败，失败时返回 SUCCESS
		if (r == BTResult::RUNNING)
			return BTResult::RUNNING;
	}
	return BTResult::SUCCESS;
}

// GameServer CBTDecoratorTimeout: 累计计时，超过 ms 则失败；ms=0 透传
BTResult BTEngine::ExecuteDecoratorTimeout(std::shared_ptr<BTNode> node)
{
	if (node->children.empty()) return BTResult::FAILURE;
	auto it = node->params.find(L"ms");
	DWORD dwTimeout = it != node->params.end() ? (DWORD)SafeStoi(it->second, 5000) : 5000;
	if (dwTimeout == 0)
		return ExecuteNode(node->children[0]);

	// 使用节点 id 作为状态键，记录起始时间
	const std::wstring key = node->id + L"_TO_START";
	DWORD dwNow = m_ctx.frameTime;
	DWORD dwStart = 0;
	auto sit = m_decoratorState.find(key);
	if (sit != m_decoratorState.end()) dwStart = sit->second;

	if (dwStart == 0)
	{
		dwStart = dwNow;
		m_decoratorState[key] = dwStart;
	}

	if (dwNow - dwStart >= dwTimeout)
	{
		m_decoratorState.erase(key);   // 超时重置
		return BTResult::FAILURE;
	}

	BTResult r = ExecuteNode(node->children[0]);
	if (r == BTResult::SUCCESS || r == BTResult::FAILURE)
		m_decoratorState.erase(key);   // 子节点结束，重置计时
	return r;
}

// GameServer CBTDecoratorCooldown: 冷却时间内返回 FAILURE
BTResult BTEngine::ExecuteDecoratorCooldown(std::shared_ptr<BTNode> node)
{
	if (node->children.empty()) return BTResult::FAILURE;
	auto it = node->params.find(L"ms");
	DWORD dwCooldown = it != node->params.end() ? (DWORD)SafeStoi(it->second, 3000) : 3000;

	const std::wstring key = node->id + L"_CD_LAST";
	DWORD dwNow = m_ctx.frameTime;
	DWORD dwLast = 0;
	auto sit = m_decoratorState.find(key);
	if (sit != m_decoratorState.end()) dwLast = sit->second;

	if (dwLast > 0 && (dwNow - dwLast) < dwCooldown)
		return BTResult::FAILURE;

	m_decoratorState[key] = dwNow;
	return ExecuteNode(node->children[0]);
}

// GameServer CBTDecoratorPeriodic: 每 ms 触发一次子节点并返回真实结果，周期内返回 FAILURE
BTResult BTEngine::ExecuteDecoratorPeriodic(std::shared_ptr<BTNode> node)
{
	if (node->children.empty()) return BTResult::FAILURE;
	auto it = node->params.find(L"ms");
	DWORD dwPeriod = it != node->params.end() ? (DWORD)SafeStoi(it->second, 30000) : 30000;
	if (dwPeriod == 0) return BTResult::FAILURE;

	const std::wstring key = node->id + L"_P_LAST";
	DWORD dwNow = m_ctx.frameTime;
	DWORD dwLast = 0;
	auto sit = m_decoratorState.find(key);
	if (sit != m_decoratorState.end()) dwLast = sit->second;

	DWORD dwElapsed = (dwLast > 0) ? (dwNow - dwLast) : dwPeriod;
	if (dwElapsed < dwPeriod)
		return BTResult::FAILURE;   // 周期未到，让上层 Selector 继续

	m_decoratorState[key] = dwNow;
	return ExecuteNode(node->children[0]);
}

BTResult BTEngine::ExecuteSucceeder(std::shared_ptr<BTNode> node)
{
	if (!node->children.empty())
		ExecuteNode(node->children[0]);
	return BTResult::SUCCESS;
}

BTResult BTEngine::ExecuteFailer(std::shared_ptr<BTNode> node)
{
	if (!node->children.empty())
		ExecuteNode(node->children[0]);
	return BTResult::FAILURE;
}

// ============================================================================
// 条件节点 - 基于 ExecutionContext 模拟判定，覆盖全部 17 种条件
// ============================================================================
BTResult BTEngine::ExecuteCondition(std::shared_ptr<BTNode> node)
{
	auto getParam = [&](const std::wstring& key, int defVal) -> int {
		auto it = node->params.find(key);
		return it != node->params.end() ? SafeStoi(it->second, defVal) : defVal;
	};

	switch (node->conditionType)
	{
	case ConditionType::LOW_HP:
		return m_ctx.hpPercent < getParam(L"percent", 30) ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::LOW_MP:
		return m_ctx.mpPercent < getParam(L"percent", 20) ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::HAS_TARGET:
		return m_ctx.hasTarget ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::IN_SAFE_AREA:
		return m_ctx.inSafeArea ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::BAG_FULL:
		return m_ctx.bagFull ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::HAS_ITEM:
		return m_ctx.hasItem ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::SKILL_READY:
		return m_ctx.skillReady ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::IS_DEAD:
		return m_ctx.isDead ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::TARGET_DISTANCE:
	{
		int nMin = getParam(L"minDist", 0);
		int nMax = getParam(L"maxDist", 16);
		return (m_ctx.targetDistance >= nMin && m_ctx.targetDistance <= nMax) ? BTResult::SUCCESS : BTResult::FAILURE;
	}
	case ConditionType::TARGET_TYPE:
		return m_ctx.targetType == getParam(L"targetType", 0) ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::HAS_BUFF:
		return m_ctx.hasBuff ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::HAS_NEARBY_PLAYER:
		return m_ctx.hasNearbyPlayer ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::MONSTER_COUNT:
	{
		int nMode = getParam(L"mode", 0);
		int nCount = getParam(L"count", 3);
		switch (nMode)
		{
		case 1:  return m_ctx.monsterCount <= nCount ? BTResult::SUCCESS : BTResult::FAILURE; // <=
		case 2:  return m_ctx.monsterCount == nCount ? BTResult::SUCCESS : BTResult::FAILURE; // ==
		default: return m_ctx.monsterCount >= nCount ? BTResult::SUCCESS : BTResult::FAILURE; // >=
		}
	}
	case ConditionType::HAS_POTION:
		// hpType=1 检查 HP 药水，hpType=0 检查 MP 药水
		return getParam(L"hpType", 1) != 0 ? (m_ctx.hasPotionHP ? BTResult::SUCCESS : BTResult::FAILURE)
		                                   : (m_ctx.hasPotionMP ? BTResult::SUCCESS : BTResult::FAILURE);
	case ConditionType::HAS_DROPPED_ITEM:
		return m_ctx.hasDroppedItem ? BTResult::SUCCESS : BTResult::FAILURE;
	case ConditionType::HP_RANGE:
	{
		int nMin = getParam(L"minPercent", 0);
		int nMax = getParam(L"maxPercent", 100);
		return (m_ctx.hpPercent >= nMin && m_ctx.hpPercent <= nMax) ? BTResult::SUCCESS : BTResult::FAILURE;
	}
	case ConditionType::TIME_OF_DAY:
		return m_ctx.timeOfDay == getParam(L"period", 0) ? BTResult::SUCCESS : BTResult::FAILURE;
	default:
		return BTResult::FAILURE;
	}
}

// ============================================================================
// 动作节点 - 调试器无真实游戏世界，依据上下文可行性给出模拟结果
// 复合节点（如 Sequence）会因动作返回 FAILURE 而提前结束，模拟结果应尽量直观
// ============================================================================
BTResult BTEngine::ExecuteAction(std::shared_ptr<BTNode> node)
{
	auto getParam = [&](const std::wstring& key, int defVal) -> int {
		auto it = node->params.find(key);
		return it != node->params.end() ? SafeStoi(it->second, defVal) : defVal;
	};

	switch (node->actionType)
	{
	// 复活：死亡后延迟 delay 毫秒才复活成功(RUNNING 等待)
	// 参数: delay(默认0) hpPercent(复活后HP) teleportHome(是否回城)
	case ActionType::REVIVE:
	{
		if (!m_ctx.isDead) return BTResult::FAILURE;
		int delay = getParam(L"delay", 0);
		if (delay <= 0) return BTResult::SUCCESS;  // 无延迟立即复活
		const std::wstring key = node->id + L"_REVIVE_START";
		DWORD dwNow = m_ctx.frameTime;
		auto sit = m_decoratorState.find(key);
		if (sit == m_decoratorState.end())
		{
			m_decoratorState[key] = dwNow;
			return BTResult::RUNNING;  // 开始等待复活
		}
		if (dwNow - sit->second >= (DWORD)delay)
		{
			m_decoratorState.erase(key);
			return BTResult::SUCCESS;  // 延迟到期,复活成功
		}
		return BTResult::RUNNING;  // 仍在等待
	}
	// 喝药：背包有对应药水才成功
	case ActionType::USE_POTION:
		return getParam(L"hpType", 1) != 0 ? (m_ctx.hasPotionHP ? BTResult::SUCCESS : BTResult::FAILURE)
		                                   : (m_ctx.hasPotionMP ? BTResult::SUCCESS : BTResult::FAILURE);
	// 使用道具/穿戴：持有物品才成功
	case ActionType::USE_ITEM:
	case ActionType::EQUIP_ITEM:
		return m_ctx.hasItem ? BTResult::SUCCESS : BTResult::FAILURE;
	// 攻击/移向目标/定向攻击/精确施法：需要有目标
	case ActionType::ATTACK:
	case ActionType::MOVE_TO_TARGET:
	case ActionType::ATTACK_DIR:
	case ActionType::SPELL_CAST:
		return m_ctx.hasTarget ? BTResult::SUCCESS : BTResult::FAILURE;
	// 使用技能：技能需就绪
	case ActionType::USE_SKILL:
		return m_ctx.skillReady ? BTResult::SUCCESS : BTResult::FAILURE;
	// 拾取：地上需有掉落物
	case ActionType::PICKUP_ITEM:
		return m_ctx.hasDroppedItem ? BTResult::SUCCESS : BTResult::FAILURE;
	// 逃跑：HP 越低越容易成功（模拟走位失误）
	case ActionType::FLEE:
		return (m_rng() % 100 < (90 - m_ctx.hpPercent / 4)) ? BTResult::SUCCESS : BTResult::FAILURE;
	// 休息：持续 duration 毫秒后成功(RUNNING 等待)
	case ActionType::REST:
	{
		int duration = getParam(L"duration", 5000);
		if (duration <= 0) return BTResult::SUCCESS;
		const std::wstring key = node->id + L"_REST_START";
		DWORD dwNow = m_ctx.frameTime;
		auto sit = m_decoratorState.find(key);
		if (sit == m_decoratorState.end())
		{
			m_decoratorState[key] = dwNow;
			return BTResult::RUNNING;  // 开始休息
		}
		if (dwNow - sit->second >= (DWORD)duration)
		{
			m_decoratorState.erase(key);
			return BTResult::SUCCESS;  // 休息完成
		}
		return BTResult::RUNNING;  // 仍在休息
	}
	// 延迟：在 [minMs,maxMs] 随机选一时间等待(RUNNING)
	case ActionType::DELAY:
	{
		int minMs = getParam(L"minMs", 200);
		int maxMs = getParam(L"maxMs", 1000);
		if (maxMs < minMs) maxMs = minMs;
		const std::wstring key = node->id + L"_DELAY_START";
		DWORD dwNow = m_ctx.frameTime;
		auto sit = m_decoratorState.find(key);
		if (sit == m_decoratorState.end())
		{
			// 首次进入:在[minMs,maxMs]随机选目标时长
			int range = maxMs - minMs + 1;
			DWORD target = (DWORD)(minMs + (range > 0 ? (m_rng() % range) : 0));
			m_decoratorState[key] = dwNow;           // 记录开始时间
			m_decoratorState[key + L"_T"] = target;   // 记录目标时长
			return BTResult::RUNNING;
		}
		DWORD start = sit->second;
		DWORD target = 1000;
		auto tit = m_decoratorState.find(key + L"_T");
		if (tit != m_decoratorState.end()) target = tit->second;
		if (dwNow - start >= target)
		{
			m_decoratorState.erase(key);
			m_decoratorState.erase(key + L"_T");
			return BTResult::SUCCESS;
		}
		return BTResult::RUNNING;
	}
	// 挖矿：持续 duration 毫秒后成功(RUNNING 等待)
	case ActionType::MINE:
	{
		int duration = getParam(L"duration", 10000);
		if (duration <= 0) return BTResult::SUCCESS;
		const std::wstring key = node->id + L"_MINE_START";
		DWORD dwNow = m_ctx.frameTime;
		auto sit = m_decoratorState.find(key);
		if (sit == m_decoratorState.end())
		{
			m_decoratorState[key] = dwNow;
			return BTResult::RUNNING;
		}
		if (dwNow - sit->second >= (DWORD)duration)
		{
			m_decoratorState.erase(key);
			return BTResult::SUCCESS;
		}
		return BTResult::RUNNING;
	}
	// 回城：读卷轴延迟 1000ms 后成功(RUNNING)
	case ActionType::RECALL:
	{
		const std::wstring key = node->id + L"_RECALL_START";
		DWORD dwNow = m_ctx.frameTime;
		auto sit = m_decoratorState.find(key);
		if (sit == m_decoratorState.end())
		{
			m_decoratorState[key] = dwNow;
			return BTResult::RUNNING;
		}
		if (dwNow - sit->second >= 1000)
		{
			m_decoratorState.erase(key);
			return BTResult::SUCCESS;
		}
		return BTResult::RUNNING;
	}
	// 其余动作（巡逻/聊天/说话/丢弃/召唤/跟随/组队/切攻击模式）：常态成功
	default:
		return BTResult::SUCCESS;
	}
}

// ============================================================================
// XML 序列化 —— 将行为树写回 XML 字符串
// ============================================================================
static void SerializeNode(std::wostringstream& out, std::shared_ptr<BTNode> node, int indent)
{
    std::wstring pad(indent * 4, L' ');
    std::wstring tag = GetTagName(node->type, node->conditionType, node->actionType);

    out << pad << L"<" << tag;
    if (!node->name.empty() && node->name != tag)
        out << L" name=\"" << node->name << L"\"";
    for (auto& p : node->params)
    {
        if (p.first == L"name") continue;
        out << L" " << p.first << L"=\"" << p.second << L"\"";
    }

    if (node->children.empty())
    {
        out << L" />\r\n";
    }
    else
    {
        out << L">\r\n";
        for (auto& child : node->children)
            SerializeNode(out, child, indent + 1);
        out << pad << L"</" << tag << L">\r\n";
    }
}

std::string XMLParser::SerializeTree(std::shared_ptr<BTNode> root, const std::wstring& treeName)
{
    if (!root) return "";

    std::wostringstream wss;
    wss << L"<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n";
    wss << L"<BehaviorTree name=\"" << treeName << L"\">\r\n";
    SerializeNode(wss, root, 1);
    wss << L"</BehaviorTree>\r\n";

    std::wstring wx = wss.str();
    // 输出纯 GBK（不带 BOM），与 GameServer 端 BotBehavior_*.xml 配置完全对齐
    // tinyxml 在 GameServer 端按 GBK 解析，避免 BOM + encoding 不一致导致乱码
    std::string result;
    // 不写入 BOM，保持与 GameServer 配置一致
    int len = WideCharToMultiByte(936, 0, wx.c_str(), (int)wx.size(), nullptr, 0, nullptr, nullptr);
    if (len > 0)
    {
        result.resize(len);
        WideCharToMultiByte(936, 0, wx.c_str(), (int)wx.size(), &result[0], len, nullptr, nullptr);
    }
    return result;
}