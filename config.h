/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx	= 3;		/* border pixel of windows */
static const int startwithgaps[]	= { 1 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]	= { 7 };	/* default gap between windows in pixels, this can be customized for each tag */
static const unsigned int snap		= 32;		/* snap pixel */
static const int swallowfloating	= 1;		/* 1 means swallow floating windows by default */
static const int showbar			= 1;		/* 0 means no bar */
static const int topbar				= 1;		/* 0 means bottom bar */
static const char *fonts[] = {
	"Hack Nerd Font Mono:monospace:size=16",
	"JoyPixels:pixelsize=14:antialias=true:autohint=true"
};
static const char dmenufont[]		= "monospace:size=16";
static const char col_gray1[]		= "#222222";
static const char col_gray2[]		= "#444444";
static const char col_gray3[]		= "#bbbbbb";
static const char col_gray4[]		= "#eeeeee";
static const char col_cyan[]		= "#005577";
static const char col_blue[]		= "#1f2d68";
static const char col_mint[]		= "#80ffcc";
static const char *colors[][3]		= {
	/*				fg		 bg		 border	*/
	[SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
	[SchemeSel]	= { col_gray4, col_blue,	col_mint	},
};

/* commands */
#define TERMINAL "st"
static char dmenumon[2]			= "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]	= { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]	= { TERMINAL, NULL };

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spfm", "-g", "144x41", "-e", "lf", NULL };
static Sp scratchpads[] = {
	/* name	cmd	*/
	{"spterm", spcmd1},
	{"spfm",	spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class			instance	title			tags mask	isfloating	isterminal	noswallow	monitor */
	{ "Gimp",			NULL,		NULL,			0,			1,			0,			 0,			-1 },
	{ "Firefox",		NULL,		NULL,			0,			0,			0,			-1,			-1 },
	{ "librewolf",		NULL,		NULL,			0,			0,			0,			-1,			-1 },
	{ "discord",		NULL,		NULL,			1,			0,			0,			-1,			 1 },
	{ "st-256color",	NULL,		NULL,			0,			0,			1,			 0,			-1 },
    { NULL,				NULL,		"Event Tester", 0,			0,			0,			 1,			-1 }, /* xev */
	{ NULL,				"spterm",	NULL,			SPTAG(0),	1,			1,			 0,			-1 },
	{ NULL,				"spfm",		NULL,			SPTAG(1),	1,			1,			 0,			-1 },
};

/* layout(s) */
static const float mfact		=	0.55;	/* factor of master area size [0.05..0.95] */
static const int nmaster		=	1;		/* number of clients in master area */
static const int resizehints	=	0;		/* 1 means respect size hints in tiled resizals */
static const int lockfullscreen	= 	1;		/* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol		arrange function */
	{ "[]=",		tile },			/* tiling with a master and slaves */
	{ "><>",		NULL },			/* no layout function means floating behavior */
	{ "[M]",		monocle }, 		/* everything stacked on top of eachother on the Z axis */
	{ "===",		bstackhoriz },	/* tiling with stacked on the Y axis */	
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,		view,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,		toggleview,		{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,		tag,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,		toggletag,		{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier		 				key					function		argument */
    { MODKEY,						XK_Return,			spawn,			{.v = termcmd } },										/* spawn terminal */
	{ MODKEY|ShiftMask, 			XK_Return,			togglescratch,	{.ui = 0} },											/* toggle scratchpad */
	{ MODKEY,						XK_space,			zoom,			{0} },													/* switch master */
	{ MODKEY|ShiftMask, 			XK_space,			togglefloating, {0} },													/* make target client floating */
	/*{ MODKEY,						XK_a,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_a,				,				},														/*	*/
	{ MODKEY,						XK_b,				spawn,			SHCMD(TERMINAL " -e lf") },								/* file Browser lf */
	{ MODKEY|ShiftMask,				XK_b,				setlayout,		{.v = &layouts[3]} },									/* vertical stack */
    /*{ MODKEY,						XK_c,				,				},														/*	*/
	/*{ MODKEY,						XK_c,				spawn,			SHCMD("xsel | xclip -sel c") },							/* Copy (put selection into clipboard) */
	/*{ MODKEY|ShiftMask,			XK_c,				,				},														/*	*/
	{ MODKEY,						XK_d,				spawn,			{.v = dmenucmd } },										/* access dmenu */
	/*{ MODKEY|ShiftMask,			XK_d,				,				},														/*	*/
    /*{ MODKEY,						XK_e,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_e,				,				},														/*	*/
	/*{ MODKEY,						XK_f,				,				},														/*	*/
	{ MODKEY|ShiftMask,				XK_f,				setlayout,		{.v = &layouts[1]} },									/* floating layout (aka normie mode) */
	/*{ MODKEY,						XK_g,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_g,				,				},														/*	*/
	{ MODKEY,						XK_h,				shiftview,		{.i = -1} },											/* change to the tag to the left */
	{ MODKEY|ShiftMask,				XK_h,				shifttag,		{.i = -1} },											/* move the selected client one tag to the left */
	{ MODKEY|ControlMask,			XK_h,				focusmon,		{.i = -1} },											/* change to the left monitor */
	{ MODKEY|ControlMask|ShiftMask,	XK_h,				tagmon,			{.i = -1} },											/* move the selected client to the left monitor */
    { MODKEY,						XK_i,				incnmaster,		{.i = -1 } },											/* decrease the number of masters */
	{ MODKEY|ShiftMask,				XK_i,				incnmaster,		{.i = +1 } },											/* increase the number of masters */
	{ MODKEY,						XK_j,				focusstack,		{.i = +1 } },											/* cycle to next client */
	{ MODKEY|ShiftMask,				XK_j,				pushdown,		{0} },													/* push current client down the stack */
	{ MODKEY,						XK_k,				focusstack,		{.i = -1 } },											/* cycle to previous client	*/
    { MODKEY|ShiftMask,				XK_k,				pushup,		 	{0} },													/* push current client up the stack */
	{ MODKEY,						XK_l,				shiftview,		{.i = +1} },											/* change to the tag to the right */
    { MODKEY|ShiftMask,				XK_l,				shifttag,		{.i = +1} },											/* move the selected client one tag to the right */
	{ MODKEY|ControlMask,			XK_l,				focusmon,		{.i = +1} },											/* change to the right monitor */
	{ MODKEY|ControlMask|ShiftMask,	XK_l,				tagmon,			{.i = +1} },											/* move the selected client to the right monitor */
	/*{ MODKEY,						XK_m,				togglescratch,	{.ui = 2} },											/*	*/
    { MODKEY|ShiftMask,				XK_m,				setlayout,		{.v = &layouts[2]} },									/* monocle layout */
	{ MODKEY,						XK_n,				spawn,			SHCMD(TERMINAL " -e newsboat") },						/* start my RSS reader */
	/*{ MODKEY|ShiftMask,			XK_n,				,				},														/*	*/
	/*{ MODKEY,						XK_o,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_o,				,				},														/*	*/
	/*{ MODKEY,						XK_p,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_p,				,				},														/*	*/
	{ MODKEY,						XK_q,				killclient,		{0} },													/* close current client */
    { MODKEY|ShiftMask,				XK_q,				spawn,			SHCMD("powermenu") },									/* power menu through dmenu */
	/*{ MODKEY,						XK_r,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_r,				,				},														/*	*/
	{ MODKEY,						XK_s,				togglesticky,	{0} },													/* toggle sticky (sticky means client is on all tags) */
	/*{ MODKEY|ShiftMask,			XK_s,				,				},														/*	*/
	{ MODKEY,						XK_t,				setlayout,		{.v = &layouts[0]} },									/* tile layout */
	/*{ MODKEY|ShiftMask,			XK_t,				,				},														/*	*/
	{ MODKEY,						XK_u,				spawn,			SHCMD("dmenuunicode doit") },								/* Type an emoji/unicode char from dmenu */
	{ MODKEY|ShiftMask,				XK_u,				spawn,			SHCMD("dmenuunicode") },								/* Copy an emoji/unicode char from dmenu */
	/*{ MODKEY,						XK_v,				,				},														/*	*/
	/*{ MODKEY,						XK_v,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_v,				,				},														/*  */
	{ MODKEY,						XK_w,				spawn,			SHCMD("$BROWSER") },									/* start browser defined in environment */
	{ MODKEY|ShiftMask,				XK_w,				spawn,			SHCMD(TERMINAL " -e sudo nmtui") },						/* start network manager */
	/*{ MODKEY,						XK_x,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_x,				,				},														/*	*/
	/*{ MODKEY,						XK_y,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_y,				,				},														/*	*/
	/*{ MODKEY,						XK_z,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_z,				,				},														/*	*/
	/*{ MODKEY,						XK_grave,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_grave,			,				},														/*	*/
	{ MODKEY,						XK_Tab,				view,			{0} },													/* go to last tag */
	{ MODKEY|ShiftMask,				XK_Tab,				swapfocus,		{0} },													/* go to last client */
	/*{ MODKEY,						XK_Escape,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Escape,			,				},														/*	*/
	/*{ MODKEY,						XK_F1,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F1,				,				},														/*	*/
	/*{ MODKEY,						XK_F2,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F2,				,				},														/*	*/
	{ MODKEY,						XK_F3,				spawn,			SHCMD(TERMINAL " -e htop") },							/* start htop */
	/*{ MODKEY|ShiftMask,			XK_F3,				,				},														/*	*/
	{ MODKEY,						XK_F4,				spawn,			SHCMD(TERMINAL " -e pulsemixer") },						/* start audio tuner */
	/*{ MODKEY|ShiftMask,			XK_F4,				,				},														/*	*/
	/*{ MODKEY,						XK_F5,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F5,				,				},														/*	*/
	/*{ MODKEY,						XK_F6,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F6,				,				},														/*	*/
	/*{ MODKEY,						XK_F7,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F7,				,				},														/*	*/
	/*{ MODKEY,						XK_F8,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F8,				,				},														/*	*/
	/*{ MODKEY,						XK_F9,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F9,				,				},														/*	*/
	/*{ MODKEY,						XK_F10,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F10,				,				},														/*	*/
	/*{ MODKEY,						XK_F11,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F11,				,				},														/*	*/
	/*{ MODKEY,						XK_F12,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_F12,				,				},														/*	*/
	{ MODKEY,						XK_Print,			spawn,			SHCMD("maim pic-full-$(date '+%y%m%d-%H%M%S').png") },	/* screenshot whole screen */
	{ MODKEY|ShiftMask,				XK_Print,			spawn,			SHCMD("maimpick") },									/* screenshot selector */
	/*{ MODKEY,						XK_Scroll_Lock,		,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Scroll_Lock,		,				},														/*	*/
	/*{ MODKEY,						XK_Pause,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Pause,			,				},														/*	*/
	/*{ MODKEY,						XK_Insert,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Insert,			,				},														/*	*/
	/*{ MODKEY,						XK_Delete,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Delete,			,				},														/*	*/
	/*{ MODKEY,						XK_Home,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Home,			,				},														/*	*/
	/*{ MODKEY,						XK_End,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_End,				,				},														/*	*/
	/*{ MODKEY,						XK_Page_Up,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Page_Up,			,				},														/*	*/
	/*{ MODKEY,						XK_Page_Down,		,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Page_Down,		,				},														/*	*/
	/*{ MODKEY,						XK_minus,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_minus,			,				},														/*	*/
	/*{ MODKEY,						XK_equal,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_equal,			,				},														/*	*/
	/*{ MODKEY,						XK_BackSpace,		,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_BackSpace,		,				},														/*	*/
	/*{ MODKEY,						XK_bracketleft,		,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_bracketleft,		,				},														/*	*/
	/*{ MODKEY,						XK_bracketright,	,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_bracketright,	,				},														/*	*/
	/*{ MODKEY,						XK_backslash,		,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_backslash,		,				},														/*	*/
	{ MODKEY,						XK_semicolon,		setmfact,		{.f = -0.05} },											/* decrease width of master client */
	{ MODKEY|ShiftMask,				XK_semicolon,		setmfact,		{.f = +0.05} },											/* increase width of master client */
	/*{ MODKEY,						XK_apostrophe,		,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_apostrophe,		,				},														/*	*/
	/*{ MODKEY,						XK_comma,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_comma,			,				},														/*	*/
	/*{ MODKEY,						XK_period,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_period,			,				},														/*	*/
	/*{ MODKEY,						XK_slash,			,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_slash,			,				},														/*	*/
	/*{ MODKEY,						XK_Up,				,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Up,				,				},														/*	*/
	{ MODKEY,						XK_Left,			 focusmon,		{.i = -1 } },											/*	*/
	{ MODKEY|ShiftMask,				XK_Left,			 tagmon,		{.i = -1 } },											/*	*/
	/*{ MODKEY,						XK_Down,			 ,				},														/*	*/
	/*{ MODKEY|ShiftMask,			XK_Down,			 ,				},														/*	*/
	{ MODKEY,						XK_Right,			focusmon,		{.i = +1 } },											/*	*/
	{ MODKEY|ShiftMask,				XK_Right,			tagmon,			{.i = +1 } },											/*	*/

	{ 0,	XF86XK_AudioMute,			spawn,	SHCMD("audio-ctl mute") },										/* mute script */
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,	SHCMD("audio-ctl 5") },											/* increase volume script */
	{ 0,	XF86XK_AudioLowerVolume,	spawn,	SHCMD("audio-ctl -5") },										/* decrease volume script */
	{ 0,	XF86XK_AudioPrev,			spawn,	SHCMD("audio-ctl previous") },									/* previous track script */
	{ 0,	XF86XK_AudioNext,			spawn,	SHCMD("audio-ctl next") },										/* next track script */
	{ 0,	XF86XK_AudioPause,			spawn,	SHCMD("audio-ctl pause") },										/* previous track script */
	{ 0,	XF86XK_AudioPlay,			spawn,	SHCMD("audio-ctl play-pause") },								/* play-pause track script */
	{ 0,	XF86XK_AudioStop,			spawn,	SHCMD("audio-ctl stop") },										/* stop track script */
	/*{ 0,	XF86XK_AudioRewind,			,		},																/*	*/
	/*{ 0,	XF86XK_AudioForward,		,		},																/*	*/
	/*{ 0,	XF86XK_AudioMedia,			,		},																/*	*/
	{ 0,	XF86XK_AudioMicMute,		spawn,	SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },		/* mute microphone in pulse audio` */
	{ 0,	XF86XK_PowerOff,			spawn,	SHCMD("powermenu") },											/* power menu through dmenu	*/
	{ 0,	XF86XK_Calculator,			spawn,	SHCMD(TERMINAL " -e bc -l") },									/* BC = Best Calculator */
	{ 0,	XF86XK_Sleep,				spawn,	SHCMD("systemctl suspend-then-hibernate") },					/* sleep */
	{ 0,	XF86XK_WWW,					spawn,	SHCMD("$BROWSER") },											/* start browser defined in environment */
	{ 0,	XF86XK_DOS,					spawn,	SHCMD(TERMINAL) },												/* start terminal */
	{ 0,	XF86XK_ScreenSaver,			spawn,	SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },	/* show screensaver */
	{ 0,	XF86XK_TaskPane,			spawn,	SHCMD(TERMINAL " -e htop") },									/* show htop */
	/*{ 0,	XF86XK_Mail,				,		},																/*	*/
	/*{ 0,	XF86XK_MyComputer,			,		},																/*	*/
	/*{ 0,	XF86XK_Battery,				,		},																/*	*/
	{ 0,	XF86XK_Launch1,				spawn,	SHCMD("xset dpms force off") },									/* off */
	{ 0,	XF86XK_TouchpadToggle,	 	spawn,	SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },	/* toggle touchpad */
	{ 0,	XF86XK_TouchpadOff,			spawn,	SHCMD("synclient TouchpadOff=1") },								/* turn off touchpad */
	{ 0,	XF86XK_TouchpadOn,		 	spawn,	SHCMD("synclient TouchpadOff=0") },								/* turn on touchpad */
	{ 0,	XF86XK_MonBrightnessUp,		spawn,	SHCMD("xbacklight -inc 10") },									/* increase brightness (only works for intel) */
	{ 0,	XF86XK_MonBrightnessDown,	spawn,	SHCMD("xbacklight -dec 10") },									/* decrease brightness (only works for intel) */

	{ MODKEY,			XK_0,	view,	{.ui = ~0 } },	/* show all tags */
	{ MODKEY|ShiftMask,	XK_0,	tag,	{.ui = ~0 } },	/* set to all tags */
	TAGKEYS(			XK_1,	0 )
	TAGKEYS(			XK_2,	1 )
	TAGKEYS(			XK_3,	2 )
	TAGKEYS(			XK_4,	3 )
	TAGKEYS(			XK_5,	4 )
	TAGKEYS(			XK_6,	5 )
	TAGKEYS(			XK_7,	6 )
	TAGKEYS(			XK_8,	7 )
	TAGKEYS(			XK_9,	8 )
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click				event mask		button			function		argument */
	/*{ ClkLtSymbol,		0,				Button1,		setlayout,		{0} }, */
	/*{ ClkLtSymbol,		0,				Button3,		setlayout,		{.v = &layouts[2]} }, */
	{ ClkWinTitle,			0,				Button2,		zoom,			{0} },
	{ ClkStatusText,		0,				Button2,		spawn,			{.v = termcmd } },
	{ ClkClientWin,			MODKEY,			Button1,		movemouse,		{0} },
	{ ClkClientWin,			MODKEY,			Button2,		togglefloating,	{0} },
	{ ClkClientWin,			MODKEY,			Button3,		resizemouse,	{0} },
	{ ClkTagBar,			0,				Button1,		view,			{0} },
	{ ClkTagBar,			0,				Button3,		toggleview,		{0} },
	{ ClkTagBar,			MODKEY,			Button1,		tag,			{0} },
	{ ClkTagBar,			MODKEY,			Button3,		toggletag,		{0} },
};

