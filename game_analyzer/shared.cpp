int lenstr(const char *str)
{
	int i = 0, length = 0;
	if (str)
	{
		while (str[i] != '\0')
		{
			length++;
			i++;
		}
	}
	return length;
}


int cmpstr(const char *str1, const char *str2)
{
	int i = 0, len = lenstr(str1);
	if (str1&&str2)
	{
		if (len == lenstr(str2))
		{
			while (i <= len)
			{
				if (str1[i] != str2[i])
					return 1;
				i++;
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
	return 0;
}