#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "opc/frl_opc_async_request.h"
#include "opc/frl_opc_com_variant.h"

namespace frl
{
	namespace opc
	{
		namespace private_
		{
			DWORD getUniqueCancelID()
			{
				static DWORD id = 0;
				return ++id;
			}
		}

		AsyncRequest::AsyncRequest()
			:	id( 0 ),
				cancelID( private_::getUniqueCancelID() ),
				cancelled( False ),
				values( NULL ),
				source( OPC_DS_DEVICE )
		{
		}

		AsyncRequest::AsyncRequest( std::list< OPCHANDLE > handles_ )
			:	id( 0 ),
				cancelID( private_::getUniqueCancelID() ),
				cancelled( False ),
				handles( handles_ ),
				values( NULL ),
				source( 0 )
		{

		}

		AsyncRequest::AsyncRequest( const AsyncRequest &rhv )
			:	id( rhv.id ),
				cancelID( rhv.cancelID ),
				cancelled( rhv.cancelled ),
				handles( rhv.handles ),
				source( rhv.source )
		{
			size_t size = handles.size();
			values = NULL;
			if( size != 0 && rhv.values != NULL )
			{
				values = new VARIANT[ size ];
				for( size_t i = 0; i < size; i++ )
				{
					::VariantInit( & values[i] );
					ComVariant::variantCopy( &values[i], &rhv.values[i] );
				}
			}
		}

		AsyncRequest::~AsyncRequest()
		{
			if( values != NULL )
				delete [] values;
			values = NULL;
		}

		AsyncRequest& AsyncRequest::operator = ( const AsyncRequest &rhv )
		{
			if( this == &rhv )
				return *this;
			cancelled = rhv.cancelled;
			cancelID = rhv.cancelID;
			id = rhv.id;
			handles = rhv.handles;
			size_t size = handles.size();
			if( values != NULL )
				delete [] values;
			values = NULL;
			if( size != 0  && rhv.values != NULL  )
			{
				values = new VARIANT[ size ];
				for( size_t i = 0; i < size; i++ )
				{
					::VariantInit( & values[i] );
					ComVariant::variantCopy( &values[i], &rhv.values[i] );
				}
			}
			return *this;
		}

		void AsyncRequest::setTransactionID( DWORD id_ )
		{
			id = id_;
		}

		DWORD AsyncRequest::getTransactionID() const
		{
			return id;
		}

		DWORD AsyncRequest::getCancelID()
		{
			return cancelID;
		}

		frl::Bool AsyncRequest::isCancelled()
		{
			return cancelled;
		}

		void AsyncRequest::isCancelled( Bool isCancelled_ )
		{
			cancelled = isCancelled_;
		}

		void AsyncRequest::init( std::list< OPCHANDLE > handles_, const VARIANT *values_ )
		{
			if( values != NULL )
				delete [] values;
			values = NULL;
			handles = handles_;
			size_t counts = handles.size();
			if( counts == 0 )
				FRL_THROW_S_CLASS( AsyncRequest::InvalidParameter );

			values = new VARIANT[ counts ];
			for( DWORD i = 0; i < counts; i++ )
			{
				::VariantInit( &values[i] );
				ComVariant::variantCopy( &values[i], &values_[i] );
			}
		}

		const std::list<OPCHANDLE>& AsyncRequest::getHandles() const
		{
			return handles;
		}

		size_t AsyncRequest::getCounts() const
 		{
			return handles.size();
		}

		const VARIANT* AsyncRequest::getValues() const
		{
			return values;
		}

		void AsyncRequest::removeHandles( OPCHANDLE handle )
		{
			size_t originalSize = handles.size();
			if( originalSize == 0 )
				return;

			std::vector< size_t > posList;
			size_t i = 0;
			for( std::list< OPCHANDLE >::iterator it = handles.begin(), remIt; it != handles.end(); it = remIt )
			{
				remIt = it;
				++remIt;
				if( (*it) == handle )
				{
					posList.push_back( i );
					handles.erase( it );
				}
			}

			size_t size = handles.size();
			if( size > 0 && values != NULL )
			{
				VARIANT *tmpVal = new VARIANT[ size ];
				for( size_t i = 0, j = 0; i < originalSize; i++ )
				{
					for( std::vector< size_t >::iterator it = posList.begin(); it != posList.end(); ++it )
					{
						if( i != (*it) )
						{
							::VariantInit( &tmpVal[ j ] );
							ComVariant::variantCopy( &tmpVal[ j ], &values[ i ] );
							++j;
						}
					}
				}
				delete [] values;
				values = tmpVal;
			}
		}

		DWORD AsyncRequest::getSource() const
		{
			return source;
		}

		void AsyncRequest::setSource( DWORD source_ )
		{
			source = source_;
		}

	} // namespace opc
} // namespace frl

#endif // FRL_PLATFORM == FRL_PLATFORM_WIN32
