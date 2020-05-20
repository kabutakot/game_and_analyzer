int lenstr(const char *str)
{
	int i = 0;
	if (str)
	{
		while (str[i] != '\0')
		{
			i++;
		}
	}
	return i;
}


int cmpstr(const char *str1, const char *str2)
{
	if (lenstr(str1) == lenstr(str2))
	{
		int i = 0;
		while (str1[i]!='\0')
		{
			if (str1[i] != str2[i])
			{
				return 1;
			}
			i++;
		}
		return 0;
	}
	return 1;
}