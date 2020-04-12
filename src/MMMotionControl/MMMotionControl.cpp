// MMMotionControl.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"


#define TEXTURE_WIDTH	(640)
#define TEXTURE_HEIGHT	(480)

#define M_PI       (3.14159265358979323846)
#define M_PI_2     (1.57079632679489661923)


const float g_fVersion = 1.30f;


IDirect3DTexture9 *g_pTexture;

D3DXQUATERNION g_vecHeadRotate;
D3DXVECTOR3 g_vecHead;

// 0:center 1:neck 2:head 3:shoulderL 4:elbowL 5:wristL 6:shoulderR 7:elbowR 8:wristR 9:legL 10:kneeL 11:ancleL 12:legR 13:kneeR 14:ancleR 15:torso 16:handL 17:handR
// 0:センター 1:首 2:頭 3:左肩 4:左ひじ 5:左手首 6:右肩 7:右ひじ 8:右手首 9:左足 10:左ひざ 11:左足首? 12:右足 13:右ひざ 14:右足首? 15:上半身? 16:左手=5 17:右手=8
D3DXVECTOR3 BP_Vector[18];

/* 現状、モデル内のボーン初期座標値を読んで下記に入力する作業が必要。 */
static const D3DXVECTOR3 g_Vector_Base[] = {
/* 0	*/	D3DXVECTOR3(0.0f, 11.14459f, 0.0f),	/* 9,12のY軸値 */
/* 1	*/	D3DXVECTOR3(0.0f, 16.66712f, 0.3646003f),
/* 2	*/	D3DXVECTOR3(0.0f, 17.19807f, 0.349824f),

/* 6	*/	D3DXVECTOR3(-0.3388936f, 16.19104f, 0.4044576f),
/* 7	*/	D3DXVECTOR3(-3.136329f, 14.23758f, 0.3642927f),
/* 8	*/	D3DXVECTOR3(-4.910939f, 12.83687f, 0.3279254f),

/* 3	*/	D3DXVECTOR3(0.3388936f, 16.19104f, 0.4044576f),
/* 4	*/	D3DXVECTOR3(3.136329f, 14.23758f, 0.3642927f),
/* 5	*/	D3DXVECTOR3(4.910939f, 12.83687f, 0.3279254f),

/* 12	*/	D3DXVECTOR3(-1.005733f, 11.14459f, 0.1749623f),
/* 13	*/	D3DXVECTOR3(-0.8786164f, 5.910791f, 0.1819153f),
/* 14	*/	// D3DXVECTOR3(-0.998143f, 0.96219f, 0.8180784f),	/* なぜかこの座標だけこうしないと動かない */
/* 14	*/	D3DXVECTOR3(-80.0f, 0.96219f, 0.8180784f),

/* 9	*/	D3DXVECTOR3(1.005733f, 11.14459f, 0.1749623f),
/* 10	*/	D3DXVECTOR3(0.8786164f, 5.910791f, 0.1819153f),
/* 11	*/	// D3DXVECTOR3(0.998143f, 0.96219f, 0.8180784f),	/* なぜかこの座標だけこうしないと動かない */
/* 11	*/	D3DXVECTOR3(80.0f, 0.96219f, 0.8180784f),

/* 15	*/	// D3DXVECTOR3(0.0f, 12.28145f, -0.2263783f),
/* 15	*/	D3DXVECTOR3(0.0f, 12.28145f, 0.0f),

/* 17	*/	D3DXVECTOR3(-4.910939f, 12.83687f, 0.3279254f),
/* 16	*/	D3DXVECTOR3(4.910939f, 12.83687f, 0.3279254f),

			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
};

const D3DXVECTOR3 g_Vector_Zero = D3DXVECTOR3(0.0f, (11.14459f + 11.14459f) / 2.0f, 0.0f);

#if _DEBUG
void PrintConsole(const char *szString, const int *pInt)
{
	char szStrBuffer[1024];
	int nCharWritten = 0;
	if( szString )
		nCharWritten = sprintf_s(szStrBuffer, sizeof(szStrBuffer), "%s", szString);
	if( pInt )
		nCharWritten += sprintf_s(szStrBuffer + nCharWritten, sizeof(szStrBuffer) - nCharWritten, "%d", *pInt);

	nCharWritten += sprintf_s(szStrBuffer + nCharWritten, sizeof(szStrBuffer) - nCharWritten, "\r\n");

	DWORD dwWritten;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), szStrBuffer, nCharWritten, &dwWritten, NULL);
}
void PrintConsole(const char *szString, const char *szString2)
{
	char szStrBuffer[1024];
	int nCharWritten = 0;
	if( szString )
		nCharWritten = sprintf_s(szStrBuffer, sizeof(szStrBuffer), "%s", szString);
	if( szString2 )
		nCharWritten += sprintf_s(szStrBuffer + nCharWritten, sizeof(szStrBuffer) - nCharWritten, "%s", szString2);

	nCharWritten += sprintf_s(szStrBuffer + nCharWritten, sizeof(szStrBuffer) - nCharWritten, "\r\n");

	DWORD dwWritten;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), szStrBuffer, nCharWritten, &dwWritten, NULL);
}
#endif

EXPORT bool EXPFUNC OpenNIInit(HWND hWnd, bool bEnglish, LPDIRECT3DDEVICE9 lpD3DDevice9, WCHAR *pPath, CHAR *pFileName)
{
	HRESULT hr;

	if( !lpD3DDevice9 ) {
		return false;
	}

#if 1
	/* lpD3DDevice9 に関連付けられた IDirect3D9 が IDirect3D9Ex を取得できるかどうかで、D3DPOOL を MANAGED or DEFAULT にする？ */
	hr = lpD3DDevice9->CreateTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTexture, NULL); /* Direct3D9 */
	// hr = lpD3DDevice9->CreateTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pTexture, NULL); /* Direct3D9Ex */
	if(FAILED(hr)) {
		return false;
	}

	D3DLOCKED_RECT rect;
	hr = g_pTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);		/* バッファの中身は一旦破棄してよい */
	if( SUCCEEDED(hr) ) {
		int i;
		int len;

		D3DCOLOR *pBuffer = (D3DCOLOR *)rect.pBits;

		len = TEXTURE_WIDTH * TEXTURE_HEIGHT;

		for(i = 0; i < len; i++ ) {
			*pBuffer = D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00);
			pBuffer++;
		}

		hr = g_pTexture->UnlockRect(0);
	}
#else
	D3DXCreateTextureFromFile(lpD3DDevice9, TEXT("Data\\Texture.bmp"), &g_pTexture);

#endif
	return true;
}

EXPORT void EXPFUNC OpenNIClean()
{
	if( g_pTexture ) {
		g_pTexture->Release();
		g_pTexture = NULL;
	}
}

EXPORT void EXPFUNC OpenNIDrawDepthMap(bool bWait)
{

}

EXPORT void EXPFUNC OpenNIDepthTexture(IDirect3DTexture9 **ppTexture)
{
	if( ppTexture )
		*ppTexture = g_pTexture;
}

EXPORT void EXPFUNC OpenNIGetSkeltonJointPosition(int nIndex, D3DXVECTOR3 *pVector)
{
	D3DXQUATERNION qn;
	D3DXMATRIX matQ;
	D3DXVECTOR4 vTrs;
	D3DXVECTOR3 vAxis;
	FLOAT fAngle;

	D3DXQuaternionNormalize(&qn, &g_vecHeadRotate);
	D3DXQuaternionToAxisAngle(&qn, &vAxis, &fAngle);
	// D3DXMatrixRotationQuaternion(&matQ, &qn);
	vAxis.z = -vAxis.z;
	D3DXMatrixRotationAxis(&matQ, &vAxis, -fAngle / 200);

	if( 0 <= nIndex && nIndex <= 17 && pVector ) {
		*pVector = g_Vector_Base[nIndex];

		if( nIndex == 0 ) {
			pVector->x += g_vecHead.x * 10;
			pVector->y += g_vecHead.y * 50;
			pVector->z += g_vecHead.z * 10;
		}
		else if( nIndex == 1 ) {
			pVector->x += g_vecHead.x * 5;
			pVector->y += g_vecHead.y * 50;
			pVector->z += g_vecHead.z * 5;
			//D3DXVec3Transform(&vTrs, pVector, &matQ);
			//pVector->x = vTrs.x;
			//pVector->y = vTrs.y;
			//pVector->z = vTrs.z;
		}
		else if( nIndex == 2 ) {
			//pVector->x += g_vecHead.x;
			//pVector->y += g_vecHead.y;
			//pVector->z += g_vecHead.z;
			D3DXVec3Transform(&vTrs, pVector, &matQ);
			pVector->x = vTrs.x + g_vecHead.x * 5;
			pVector->y = vTrs.y + g_vecHead.y * 50;
			pVector->z = vTrs.z + g_vecHead.z * 5;

			D3DXMatrixRotationAxis(&matQ, &vAxis, fAngle);
			D3DXVECTOR3 vecSub;
			D3DXVec3Subtract(&vecSub, &g_Vector_Base[2], &g_Vector_Base[1]);
			D3DXVec3Transform(&vTrs, &vecSub, &matQ);
			D3DXVECTOR3 v = D3DXVECTOR3(vTrs.x,vTrs.y,vTrs.z);
			D3DXVec3Add(pVector, &v, &g_Vector_Base[1]);
		}

		//if( nIndex != 0 )
		//	pVector->y -= g_Vector_Zero.y;

		if( nIndex != 2 )
			*pVector = g_Vector_Base[nIndex];

#if 0
		if( false && nIndex == 0 ) {
			g_Vector_Base[nIndex].y -= 0.5f;
			if( g_Vector_Base[nIndex].y <= -10.0f )
				g_Vector_Base[nIndex].y = 20.0f;
		}
#endif
	}

}

EXPORT void EXPFUNC OpenNIIsTracking(bool *pIsTracking)
{
	if( pIsTracking )
		*pIsTracking = true;
}

EXPORT void EXPFUNC OpenNIGetVersion(float *pVersion)
{
	if( pVersion )
		*pVersion = g_fVersion;
}

EXPORT_C void EXPFUNC MMMotionControl_SetHeadPoint(D3DXQUATERNION *pHeadRotate, D3DXVECTOR3 *pHeadPoint)
{
	if( pHeadRotate )
		g_vecHeadRotate = *pHeadRotate;

	if( pHeadPoint )
		g_vecHead = *pHeadPoint;

#if _DEBUG
	TCHAR tstrBuffer[1025] = L"";
	DWORD dwLength,dwWritten = 0;

	dwLength = (DWORD)swprintf_s(tstrBuffer, 1024, L"Head: x=%f, y=%f, z=%f, w=%f\r\n",
		g_vecHeadRotate.x, g_vecHeadRotate.y, g_vecHeadRotate.z, g_vecHeadRotate.w);

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleW(h, tstrBuffer, dwLength, &dwWritten, NULL);

	D3DXQUATERNION qn;
	FLOAT fAngle;
	D3DXVECTOR3 vAxis;
	D3DXQuaternionNormalize(&qn, &g_vecHeadRotate);
	D3DXQuaternionToAxisAngle(&qn, &vAxis, &fAngle);

	dwLength = (DWORD)swprintf_s(tstrBuffer, 1024, L"Rotate: x=%f, y=%f, z=%f, angle=%f\r\n",
		vAxis.x, vAxis.y, vAxis.z, fAngle);
	WriteConsoleW(h, tstrBuffer, dwLength, &dwWritten, NULL);

#endif
}

