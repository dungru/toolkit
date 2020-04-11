int repeatedStringMatch(char* A, char* B) 
{
    int iLenB = strlen(B);
    int iLenA = strlen(A);
    
    int iMax = iLenB / iLenA + 2;
    char* pStr = (char*) malloc(iMax * iLenA + 1);
    for (int i = 0; i < iMax; i++)
    {
        memcpy(&pStr[iLenA * i], A, iLenA);
    }
    pStr[iLenA * iMax] = '\0';
    
    char* pStr2 = strstr(pStr, B);
    if (pStr2 == NULL)
        return -1;
    
    return ceil(((pStr2 - pStr) + (float) iLenB) / (float) iLenA);
}