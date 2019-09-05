#pragma once

#ifndef UCOMMON_H
#define UCOMMON_H

// 拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。このワーニングを抑制する。
#pragma warning(disable: 4238)

#include <windows.h>
#include <tchar.h>
#include <memory>

// メモリリーク発生時にデバッガに出力する内容をわかりやすくする
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC                            // mallocによるメモリリーク検出でCPPファイル名と行数出力指定
#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new によるメモリリーク検出でCPPファイル名と行数出力指定
#else
#define NEW  new
#endif

// メモリ解放
#define SAFE_RELEASE(x) if( x != nullptr ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != nullptr ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)  if( x != nullptr ){ delete[] x;  x = nullptr; }
#define SAFE_FREE(x)  if( x != nullptr ){ free( x );  x = nullptr; }

#endif
