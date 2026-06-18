# Repository Guidelines

## Project Structure & Module Organization
`MirWorldServer_xLib/MirWorld_Server` contains the main Visual Studio solution and all C++ server components: `GameServer`, `LoginServer`, `DBServer`, `SelectCharServer`, `ServerCenter`, `StartServer`, plus shared libraries in `Public` and `xLib`. Runtime data lives under `Work/`, especially `Work/Data` for scripts, NPC logic, monster spawns, maps, and config files. `Tools/` holds standalone Delphi editors such as `MonsterEditor`, `ItemEditor`, and `NpcPageEditor`. `Publish/` contains packaged assets and operator documents, not source code.

## Build, Test, and Development Commands
Build the server suite from the solution root:

```powershell
msbuild .\MirWorldServer_xLib\MirWorld_Server\MirWorld_Server.sln /p:Configuration=Debug /p:Platform=Win32
msbuild .\MirWorldServer_xLib\MirWorld_Server\MirWorld_Server.sln /p:Configuration=Debug /p:Platform=x64
```

Debug builds output executables to `Work\Bin\`; `StartServer` outputs to `Work\`. Build the Delphi monster editor with:

```powershell
msbuild .\Tools\MonsterEditor\MonsterEditor.dproj /p:Config=Release
```

Run the stack locally by launching `Work\StartServer.exe` after verifying `Work\Config.ini` and database settings.

## Coding Style & Naming Conventions
Match the existing style exactly. In C++, use the repository’s current conventions: tabs or legacy indentation where already present, header/source pairs, and PascalCase type names such as `ServerManager`. Avoid broad refactors in legacy modules. For data files, preserve existing naming and folder conventions, including Chinese content paths like `Work\Data\Script\城市NPC\中州\`. Keep file encodings unchanged when editing script or config assets.

## Testing Guidelines
There is no dedicated automated test suite in this repository. Validate C++ changes by building the affected project and, when relevant, starting the server stack through `StartServer`. Validate data-only changes by loading the corresponding server and exercising the affected NPC, map, or script path in a local environment. Keep changes small enough that manual verification is clear.

## Commit & Pull Request Guidelines
The current history starts with a minimal `Initial commit`, so adopt short, imperative commit messages such as `Fix GameServer packet parsing` or `Adjust 中州 NPC script reward`. Keep each commit focused on one subsystem. Pull requests should describe the affected module, required config or data updates, manual verification steps, and screenshots only when a Delphi tool UI changed.

## Configuration & Data Safety
Treat `Work/` as production-like runtime state. Do not delete, rename, or mass-format data directories without confirming server impact. When changing `Work\Data\Config`, `Work\Data\Script`, or spawn tables, document the gameplay or operational effect in the PR.
