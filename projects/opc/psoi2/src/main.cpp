#include <Windows.h>
#include "main_func.h"
#include "global_var.h"
#include "util.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE )
		frl::String cmd_line = frl::string2wstring( lpCmdLine );
	#else
		frl::String cmd_line = lpCmdLine;
	#endif

	if( util::lineHelper( cmd_line ) )
		return 0;

	if( util::isAlreadyRunning() )
		return 0;

	if( ! regClassHelper( hInstance ) )
	{
		frl::String msg_string = frl::sys::util::getLastErrorDescription();
		MessageBox( NULL, msg_string.c_str(), FRL_STR("PSOI2 OPC server: Main window registration error"), MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	{
		frl::poor_xml::Document config;
		try
		{
			config.loadFromCurrenttDir( FRL_STR( "config.xml" ) );
			frl::String sw = config.getRoot()->getProprtyVal( FRL_STR("ShowWindow") );
			if( frl::lexicalCast<String, int>( sw ) == 0 )
			{
				initInstance( hInstance, SW_HIDE );
			}
			else
			{
				initInstance( hInstance, nShowCmd );
			}
			frl::String isExit = config.getRoot()->getProprtyVal( FRL_STR("ExitOnAllClientDisconnected") );
			global_var::exit_if_all_client_disconnected = frl::lexicalCast< String, Bool >( isExit );
		}
		catch( frl::Exception& ){}
	}

	if( global_var::exit_if_all_client_disconnected )
		createCheckConnectionTimer();

	MSG msg;
	while( GetMessage( &msg, 0, 0, 0 ) )
		DispatchMessage( &msg );

	return msg.wParam;
}
