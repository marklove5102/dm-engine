#pragma once

enum    {DES_ENCRYPT,DES_DECRYPT};

class CDes
{
public:
	CDes(void);
	virtual ~CDes(void);

	bool Des_Go(char *Out, const char *In, long datalen, const char *Key, int keylen, bool Type);
};
