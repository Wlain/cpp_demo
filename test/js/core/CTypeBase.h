﻿/**@file  		CTypeBase.h
* @brief		Information of c++ type
* @author		Daphnis Kau
* @date			2019-06-24
* @version		V1.0
*/

#ifndef __TYPE_BASE_H__
#define __TYPE_BASE_H__

#include <stddef.h>
#include <vector>
#include "XScriptDef.h"

namespace XS
{
	typedef ptrdiff_t DataType;
	class CTypeBase { protected: virtual ~CTypeBase() {} };

	DataType		ToDataType( const STypeInfo& argTypeInfo );
	size_t			GetSizeOfType( DataType nType );
	size_t			GetAligenSizeOfType( DataType nType );

	inline bool IsValueClass( DataType nType )
	{
		return nType > eDT_enum && !( nType & 1 );
	}

	class CGlobalTypes
	{
		CTypeBase*	m_aryTypes[eDT_count];
	public:
		CGlobalTypes(
			CTypeBase* pCharType,
			CTypeBase* pInt8Type,
			CTypeBase* pInt16Type,
			CTypeBase* pInt32Type,
			CTypeBase* pInt64Type,
			CTypeBase* pLongType,
			CTypeBase* pUint8Type,
			CTypeBase* pUint16Type,
			CTypeBase* pUint32Type,
			CTypeBase* pUint64Type,
			CTypeBase* pUlongType,
			CTypeBase* pWCharType,
			CTypeBase* pBoolType,
			CTypeBase* pFloatType,
			CTypeBase* pDoubleType,
			CTypeBase* pStringType,
			CTypeBase* pWStringType,
			CTypeBase* pPointerType,
			CTypeBase* pClassPointType,
			CTypeBase* pClassValueType );

		CTypeBase* GetTypeBase( DataType eType )
		{
			if( eType == eDT_void )
				return nullptr;
			if( eType < eDT_count )
				return m_aryTypes[eType - 1];
			if( eType & 1 )
				return m_aryTypes[eDT_count - 2];
			return m_aryTypes[eDT_count - 1];
		}

		template<class ImpClass>
		ImpClass* GetTypeImp( DataType eType )
		{
			return static_cast<ImpClass*>( GetTypeBase( eType ) );
		}
	};

	#define GlobalTypeTemplateArgs( \
		Template, ClassPointerType, ClassValueType ) \
		&Template<char>::GetInst(), \
		&Template<int8>::GetInst(), \
		&Template<int16>::GetInst(), \
		&Template<int32>::GetInst(), \
		&Template<int64>::GetInst(), \
		&Template<long>::GetInst(), \
		&Template<uint8>::GetInst(), \
		&Template<uint16>::GetInst(), \
		&Template<uint32>::GetInst(), \
		&Template<uint64>::GetInst(), \
		&Template<ulong>::GetInst(), \
		&Template<wchar_t>::GetInst(), \
		&Template<bool>::GetInst(), \
		&Template<float>::GetInst(), \
		&Template<double>::GetInst(), \
		&Template<const char*>::GetInst(), \
		&Template<const wchar_t*>::GetInst(), \
		&Template<void*>::GetInst(), \
		&ClassPointerType::GetInst(), \
		&ClassValueType::GetInst()
}

#endif
