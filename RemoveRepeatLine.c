/**********************************************************************
* 版权所有 (C)2015, Zhou Zhaoxiong。
*
* 文件名称：RemoveRepeatLine.c
* 文件标识：无
* 内容摘要：去除源文件中的重复行
* 其它说明：无
* 当前版本：V1.0
* 作    者：Zhou Zhaoxiong
* 完成日期：20151209
*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 数据类型重定义
typedef unsigned char   UINT8;
typedef signed   int    INT32;
typedef unsigned int    UINT32;

// 存放文件每行内容的链表
typedef struct T_FileInfoStruct
{
    UINT8  szContentLine[256];
    struct T_FileInfoStruct  *pNext;
} T_FileInfo;


// 函数声明
void RemRepLineAndWriResFile(UINT8 *pszTestFile);
void RmNewLine(UINT8 *pInStr);
UINT32 IsInList(T_FileInfo *ptContentListHead, UINT8 *pszContentLine);
void WriteToFile(T_FileInfo *ptContentListHead);
void ClearList(T_FileInfo *ptContentListHead);
void RemoveSpaceInStr(UINT8 *pszStr, UINT32 iStrLen);


/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期       版本号        修改人            修改内容
* ---------------------------------------------------------------
* 20151209        V1.0     Zhou Zhaoxiong          创建
***********************************************************************/
INT32 main()
{	
    UINT8  szTestFile[128] = {0};

    // 拼装配置文件路径
    snprintf(szTestFile, sizeof(szTestFile)-1, "%s/zhouzhaoxiong/zzx/RemoveRepeatLine/TestFile/TestFile.txt", getenv("HOME"));

    RemRepLineAndWriResFile(szTestFile);   // 调用函数完成去重及写文件的操作

    return 0;              
}


/**********************************************************************
* 功能描述：去除源文件中的重复行, 并将去重之后的内容写入结果文件中
* 输入参数：pszTestFile-带路径的测试文件名
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期       版本号        修改人            修改内容
* ---------------------------------------------------------------
* 20151209        V1.0     Zhou Zhaoxiong          创建
***********************************************************************/
void RemRepLineAndWriResFile(UINT8 *pszTestFile)
{
    UINT8      szContentLine[256] = {0};
    UINT32     iLineLen           = 0;
    UINT32     iRetVal            = 0;
	
    FILE       *fp                = NULL; 
    T_FileInfo *ptContentListHead = NULL;
    T_FileInfo *ptContentListTail = NULL;
    T_FileInfo *ptCurrentContent  = NULL; 
    
    if (pszTestFile == NULL)
    {
        printf("RemRepLineAndWriResFile: pszTestFile is NULL!\n");
	return;
    }
	
    printf("RemRepLineAndWriResFile: now, begin to process file %s\n", pszTestFile);
	
    if ((fp = fopen(pszTestFile, "r")) == NULL) 
    {
        printf("RemRepLineAndWriResFile: open file %s failed!\n", pszTestFile);
        return;
    }
    else
    {
        ptContentListHead = NULL;
	ptContentListTail = NULL; 
		
	while (feof(fp) == 0 && ferror(fp) == 0)
        {
            memset(szContentLine, 0x00, sizeof(szContentLine));
            if (fgets(szContentLine, sizeof(szContentLine), fp) == NULL)    // 从源文件中读取一行内容
            {
                printf("RemRepLineAndWriResFile: get line null, break.\n");
		break;
            }
            else
            {
                printf("RemRepLineAndWriResFile: get content line: %s\n", szContentLine);
            }

            RmNewLine(szContentLine);      // 去掉字符串后面的回车换行符
            RemoveSpaceInStr(szContentLine, strlen(szContentLine));  // 去掉字符串中的空格

            iLineLen = strlen(szContentLine); 
            if (iLineLen == 0)        // 如果该行的有效长度为0, 则继续读取下一行
            {
                printf("RemRepLineAndWriResFile: the length of line is 0, continue to read the next content line.\n");
                continue;
            }
			
	    if (ptContentListHead != NULL)    // 判断当前行是否已经存在了
	    {
	        iRetVal = IsInList(ptContentListHead, szContentLine);
	        if (iRetVal == 1)   // 已经存在
		{
		    printf("RemRepLineAndWriResFile: this content line has already existed.\n");
                    continue;
		}
	    }
			
	    // 将当前行加入链表中
	    ptCurrentContent = (T_FileInfo *)malloc(sizeof(T_FileInfo));
            if (ptCurrentContent == NULL)
            {
                printf("RemRepLineAndWriResFile: exec malloc failed, memory may be not enough.\n");
                return;
            }

            memcpy(ptCurrentContent->szContentLine, szContentLine, strlen(szContentLine));

            if (ptContentListHead == NULL)   // 链表为空时作为链表头
            { 
                ptContentListHead = ptCurrentContent;
                ptContentListTail = ptCurrentContent;
            }
            else
            {
                if (ptContentListTail != NULL)  // 插入链表尾部
                {
                    ptContentListTail->pNext = ptCurrentContent;
                    ptContentListTail        = ptCurrentContent;
                }
            }
	}
	// 源文件使用完毕
        fclose(fp);
        fp = NULL;
    }
			
    // 将去重之后的结果写入文件中
    WriteToFile(ptContentListHead);
	
    // 清空链表
    ClearList(ptContentListHead);   
    ptContentListHead = NULL;
}


/**********************************************************************
* 功能描述：去掉字符串后面的回车换行符
* 输入参数：pInStr-输入字符串
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期       版本号        修改人            修改内容
* ---------------------------------------------------------------
* 20151209        V1.0     Zhou Zhaoxiong          创建
***********************************************************************/
void RmNewLine(UINT8 *pInStr)
{
    UINT32  iStrLen = 0;
	
    if (pInStr == NULL)
    {
        printf("RmNewLine: pInStr is NULL!\n");
	return;
    }

    iStrLen = strlen(pInStr);
    while (iStrLen > 0)
    {
        if (pInStr[iStrLen-1] == '\n' || pInStr[iStrLen-1] == '\r')
	{
            pInStr[iStrLen-1] = '\0';
	}
        else
	{
            break;
	}

        iStrLen --;
    }
}


/**********************************************************************
* 功能描述：判断某行内容是否已经存在于链表中了
* 输入参数：pInStr-输入字符串
* 输出参数：无
* 返 回 值：1-存在  0-不存在
* 其它说明：无
* 修改日期       版本号        修改人            修改内容
* ---------------------------------------------------------------
* 20151209        V1.0     Zhou Zhaoxiong          创建
***********************************************************************/
UINT32 IsInList(T_FileInfo *ptContentListHead, UINT8 *pszContentLine)
{
    T_FileInfo  *pTmpInfo = ptContentListHead;
	
    if (ptContentListHead == NULL || pszContentLine == NULL)
    {
        printf("IsInList: input parameter(s) is NULL!\n");
	return 0;
    }
	
    while (pTmpInfo != NULL)
    {
        if (strncmp(pszContentLine, pTmpInfo->szContentLine, strlen(pszContentLine)) == 0)   // 存在于链表中
        {
            return 1;
        }
        pTmpInfo = pTmpInfo->pNext;   
    }

    if (pTmpInfo == NULL)    // 不存在于链表中
    {
        return 0;
    }
}


/**********************************************************************
 * 功能描述： 把内容写到文件中
 * 输入参数： ptContentListHead-文件记录链表
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期            版本号            修改人           修改内容
 * ----------------------------------------------------------------------
 * 20151209             V1.0          Zhou Zhaoxiong        创建
 ************************************************************************/
void WriteToFile(T_FileInfo *ptContentListHead)
{
    FILE   *fp                  = NULL;
    UINT8   szLocalFile[500]    = {0};
    UINT8   szContentBuf[256]   = {0};
	
    if (ptContentListHead == NULL)
    {
        printf("WriteToFile: input parameter is NULL!\n");
	return;
    }
    
    snprintf(szLocalFile, sizeof(szLocalFile)-1, "%s/zhouzhaoxiong/zzx/RemoveRepeatLine/TestFile/ResultFile.txt", getenv("HOME"));
    fp = fopen(szLocalFile, "a+");
    if (fp == NULL)
    {
         printf("WriteToFile: open local file failed, file=%s\n", szLocalFile);
         return;
    }
    
    while (ptContentListHead != NULL)  
    {
        memset(szContentBuf, 0x00, sizeof(szContentBuf));
        strncpy(szContentBuf, ptContentListHead->szContentLine, strlen(ptContentListHead->szContentLine));
        printf("WriteToFile: LocalFile=%s, ContentBuf=%s\n", szLocalFile, szContentBuf);

        fputs(szContentBuf, fp);
	fputs("\r\n", fp);     // 加上回车换行符
        fflush(fp);
		
	ptContentListHead = ptContentListHead->pNext;   
    }

    fclose(fp);
    fp = NULL;
	
    return;
}


/**********************************************************************
* 功能描述： 清除链表
* 输入参数： ptContentListHead-链表指针
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期        版本号        修改人          修改内容
* ------------------------------------------------------
* 20151209        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
void ClearList(T_FileInfo *ptContentListHead)
{
    T_FileInfo *ptContentList = NULL;
    T_FileInfo *pTmpData      = NULL;
	
    if (ptContentListHead == NULL)
    {
        printf("ClearList: input parameter is NULL!\n");
	return;
    }

    ptContentList = ptContentListHead;
    while (ptContentList != NULL)
    {
        pTmpData = ptContentList;
        ptContentList  = ptContentList->pNext;
        free(pTmpData);
    }
}


/**********************************************************************
* 功能描述： 清除字符串中的空格
* 输入参数： pszStr-输入的字符串
             iStrLen-最大长度
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期        版本号     修改人            修改内容
* ------------------------------------------------------
* 20151209        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
void RemoveSpaceInStr(UINT8 *pszStr, UINT32 iStrLen)
{
    UINT8  szResult[256] = {0};
    UINT8  szBuf[256]    = {0};      
    UINT32 iLoopFlagSrc  = 0;
    UINT32 iLoopFlagDst  = 0;       

    if (pszStr == NULL)
    {
        return;
    }

    memcpy(szBuf, pszStr, iStrLen);

    for (iLoopFlagSrc = 0; iLoopFlagSrc < strlen(szBuf); iLoopFlagSrc ++)
    {
        if (szBuf[iLoopFlagSrc] != ' ')
        {
            szResult[iLoopFlagDst] = szBuf[iLoopFlagSrc];
            iLoopFlagDst ++;
        }
    }

    szResult[iLoopFlagDst+1] = 0;

    memcpy(pszStr, szResult, iStrLen);

    return;
}
