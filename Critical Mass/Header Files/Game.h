#ifndef GAME_H_
#define GAME_H_
#include <vector>
#include "Timer.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Input.h"
//SoundManager includes....................................	
#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/StateManager.h"

//enum eGameStates{ INTRO_STATE, MAINMENU_STATE,GAMEPLAY_STATE,DIFFICULTY_STATE, OPTIONS_STATE, CREDITS_STATE,PAUSE_STATE,RETRY_STATE, ANIM_STATE, NUM_STATES};

#define TIMER_SLOW_RATE 4.0f

class IState;
class CRenderNode;

struct tGamma
{
	unsigned short	m_GammaColorRed[256];		//LPVOID
	unsigned short	m_GammaColorGreen[256];		//LPVOID
	unsigned short	m_GammaColorBlue[256];		//LPVOID
};

class CGame
{
	//std::vector<IState*>		m_CurrState;
	CTimer						m_cTimer;
	HDC							m_HDC;
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;
	
	CRenderer					m_renderer;
	CObjectFactory				m_cOF;
	CObjectManager				m_cOM;
	CInput						m_Input;
	float						m_fDelta;

	// SoundManager
	Entity3D			m_Entity;
	AudioSystemWwise	m_Sound;
	tGamma				m_GammaArray;
	tGamma				m_GammaReturn;

	StateManager		m_stateManager;
	bool				m_isClosing;
	bool				m_UsingController;
	int					m_Difficulty;

	// For time slow
	float m_SlowTimer;

	float m_AltEnterTimer;
	POINT m_DefaultResolution;
			
public:

	CGame( void );
	~CGame (void );
	CGame( const CGame& );				
	CGame& operator=( const CGame& );
	static CGame* GetInstance( void );

	void Initialize( HINSTANCE _hInstance, HWND _hWnd, HDC _hdc );
	void Shutdown( void );
	bool Input( void );
	void Update( void );
	void Render( void );
	bool Play( void );

	// Renderer Stuff
	inline GLuint GetScreenWidth( void )						{ return m_renderer.GetScreenWidth();			}
	inline GLuint GetScreenHeight( void )						{ return m_renderer.GetScreenHeight();			}
	inline GLuint GetFullScreen( void )							{ return m_renderer.GetFullScreen();			}
	inline void ResizeWindow( GLsizei _width, GLsizei _height ) { m_renderer.ResizeWindow( _width, _height );	}
	inline void AddToRenderList( CRenderNode* _pRenderNode )	{ m_renderer.AddToRenderList( _pRenderNode );	}
	inline void DrawRenderList( void )							{ m_renderer.DrawRenderList();					}
	inline void UpdateRenderer( Vec3f _cameraPos, Matrix3x3f _cameraOrientation, float _elapsedTime )
	{
		m_renderer.Update( _cameraPos, _cameraOrientation, _elapsedTime );
	}
	void ChangeResolution( unsigned int _width, unsigned int _height );


	bool			GetUsingController(void) { return m_UsingController; }
	void			SetUsingController(bool _b) { m_UsingController = _b; }
	
	// Accessors
	inline bool		IsClosing( void )		{ return m_isClosing;		}
	float			GetDelta( void ) const	{ return m_fDelta;			}
	CObjectFactory* GetOF( void )			{ return &m_cOF;			}
	CObjectManager* GetOM( void )			{ return &m_cOM;			}
	CRenderer*		GetRenderer( void )		{ return &m_renderer;		}
	StateManager*	GetStateManager( void )	{ return &m_stateManager;	}
	inline HDC		GetHDC( void )			{ return m_HDC;				}
	int				GetDifficulty()			{return m_Difficulty;		}

	// Mutators
	inline void SetIsClosing( bool _newBool )	{ m_isClosing = _newBool; }
	inline void SetSlowTimer( float _timer )	{ m_SlowTimer = _timer; }
	inline void SetDifficulty( int _diff )		{ m_Difficulty = _diff; }

	static void Gamma( double _gamma, tGamma &_gammaArray, HDC _hdc );
};


#endif