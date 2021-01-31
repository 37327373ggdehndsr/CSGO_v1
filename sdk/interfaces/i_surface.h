#pragma once

struct vertex_t
{
	vertex_t( ) { }
	vertex_t( const vec2_t& pos, const vec2_t& coord = vec2_t( 0, 0 ) )
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	void Init( const vec2_t& pos, const vec2_t& coord = vec2_t( 0, 0 ) )
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	vec2_t m_Position;
	vec2_t m_TexCoord;
};

enum font_flags
{
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800
};

class i_surface {
public:

	VFUNC(unlock_cursor(), 66, void(__thiscall*)(void*))
	VFUNC(lock_cursor(), 67, void(__thiscall*)(void*))

	int create_texture( bool procedural = false )
	{
		typedef int( __thiscall* Fn )( void*, bool );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 43 ) )( this, procedural );
	}

	void textured_polygon( int n, vertex_t* pVertice, bool bClipVertices = true )
	{
		typedef void( __thiscall* Fn )( void*, int, vertex_t*, bool );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 106 ) )( this, n, pVertice, bClipVertices );
	}

	bool text_size( int Font, const wchar_t* _Input, int& Wide, int& Tall )
	{
		typedef bool( __thiscall* Fn )( void*, int, const wchar_t*, int&, int& );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 79 ) )( this, Font, _Input, Wide, Tall );
	}

	void filled_rect( int x, int y, int x2, int y2 )
	{
		typedef void( __thiscall* Fn )( void*, int, int, int, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 16 ) )( this, x, y, x2, y2 );
	}

	void outlined_rect( int x, int y, int x2, int y2 )
	{
		typedef void( __thiscall* Fn )( void*, int, int, int, int );
		return ( memory::get_vfunc<Fn>( this, 18 ) )( this, x, y, x2, y2 );
	}

	void line( int x, int y, int x2, int y2 )
	{
		typedef void( __thiscall* Fn )( void*, int, int, int, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 19 ) )( this, x, y, x2, y2 );
	}

	void print_text( const wchar_t* Text, int Len, int DrawType = 0 )
	{
		typedef void( __thiscall* Fn )( void*, wchar_t const*, int, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 28 ) )( this, Text, Len, DrawType );
	}

	void set_text_font( unsigned int Font )
	{
		typedef void( __thiscall* Fn )( void*, unsigned int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 23 ) )( this, Font );
	}
	void set_text_color( int r, int g, int b, int a )
	{
		typedef void( __thiscall* Fn )( void*, int, int, int, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 25 ) )( this, r, g, b, a );
	}
	void set_text_pos( int x, int y )
	{
		typedef void( __thiscall* Fn )( void*, int, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 26 ) )( this, x, y );
	}

	void set_text_color( col_t Color )
	{
		typedef void( __thiscall* Fn )( void*, col_t );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 24 ) )( this, Color );
	}

	void set_color( int r, int g, int b, int a )
	{
		typedef void( __thiscall* Fn )( void*, int, int, int, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 15 ) )( this, r, g, b, a );
	}

	void set_color( col_t color )
	{
		typedef void( __thiscall* Fn )( void*, col_t );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 14 ) )( this, color );
	}

	void set_texture( int id )
	{
		typedef void( __thiscall* Fn )( void*, int );
		return ( ( Fn )memory::get_vfunc<Fn>( this, 38 ) )( this, id );
	}

	unsigned long create_font( )
	{
		typedef unsigned int( __thiscall* OriginalFn )( PVOID );
		return memory::get_vfunc< OriginalFn >( this, 71 )( this );
	}

	void set_font_glyph_set( unsigned long& font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, unsigned long, const char*, int, int, int, int, int, int, int );
		memory::get_vfunc< OriginalFn >( this, 72 )( this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0 );
	}

	void play_sound( const char* fileName )
	{
		typedef void( __thiscall* OriginalFn )( void*, const char* );
		return memory::get_vfunc<OriginalFn>( this, 82 )( this, fileName );
	}
};