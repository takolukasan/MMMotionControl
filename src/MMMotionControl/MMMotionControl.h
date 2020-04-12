#ifndef _MMMotionControl_H
#define _MMMotionControl_H

/* extern "C" ÇÕÇ¢ÇÁÇ»Ç¢ÇÁÇµÇ¢ÅBÇøÇÂÇ¡Ç∆ãCéùÇøÇÌÇËÇ°ÅB */
#define EXPORT __declspec(dllexport)
#define EXPFUNC __stdcall

#define EXPORT_C extern "C" __declspec(dllexport)

EXPORT bool EXPFUNC OpenNIInit(HWND hWnd, bool bEnglish, LPDIRECT3DDEVICE9 lpD3DDevice9, WCHAR *pPath, CHAR *pFileName);
EXPORT void EXPFUNC OpenNIClean();
EXPORT void EXPFUNC OpenNIDrawDepthMap(bool bWait);
EXPORT void EXPFUNC OpenNIDepthTexture(IDirect3DTexture9 **ppTexture);
EXPORT void EXPFUNC OpenNIGetSkeltonJointPosition(int nIndex, D3DXVECTOR3 *pVector);
EXPORT void EXPFUNC OpenNIIsTracking(bool *pIsTracking);
EXPORT void EXPFUNC OpenNIGetVersion(float *pVersion);


#endif // _MMMotionControl_H
