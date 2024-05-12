/* See LICENSE file for copyright and license details. */

/* -- Constants -- */
/* commands */
#define TERMINAL 	    "st"
#define BROWSER 	    "firefox"
#define BACKGROUND 	    "bg"
#define MUSIC 		    "psst-gui"
#define SPMUSIC 	    "spmusic"
#define EMOJI_PICKER        "emojipick"
#define CALCULATOR 	    "bc"
#define LAUNCHER            "dmenu_run"
/* scratchpads */
#define SPTERM 		    "spterm"
#define SPCALC 		    "spcalc"
#define XEVTITLE 	    "Event Tester"
/* X window classes */
#define TERMCLASS           "St"
#define MUSICCLASS          "Psst-gui"
/* key aliases */
#define MODKEY              Mod4Mask /* SUPER */
/* fonts */
#define FONT                "monospace:size=10"
#define TERM_FONT           "monospace:size=16"
#define ICON_FONT           "Twemoji:pixelsize=10:antialias=true:autohint=true"

/* -- Appearance -- */
static unsigned int borderpx                = 3;   /* border pixel of windows */
static unsigned int snap                    = 32;  /* snap pixel */
static unsigned int gappih                  = 20;  /* horiz inner gap between windows */
static unsigned int gappiv                  = 10;  /* vert inner gap between windows */
static unsigned int gappoh                  = 10;  /* horiz outer gap between windows and screen edge */
static unsigned int gappov                  = 30;  /* vert outer gap between windows and screen edge */
static int swallowfloating                  = 0;   /* 1 means swallow floating windows by default */
static int smartgaps                        = 0;   /* 1 means no outer gap when there is only one window */
static int showbar                          = 1;   /* 0 means no bar */
static int topbar                           = 1;   /* 0 means bottom bar */
static const unsigned int systraypinning    = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft     = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing    = 2;   /* systray spacing */
static const int systraypinningfailfirst    = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray                = 1;   /* 0 means no systray */
static char *fonts[]                        = { FONT, ICON_FONT };
static char normbgcolor[]                   = "#282a36";
static char normbordercolor[]               = "#444444";
static char normfgcolor[]                   = "#bbbbbb";
static char selfgcolor[]                    = "#eeeeee";
static char selbordercolor[]                = "#770000";
static char selbgcolor[]                    = "#005577";
static char *colors[][3]                    = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* -- Scratchpads -- */
typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n",     SPTERM,  "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n",     SPCALC,  "-f", TERM_FONT, "-g", "50x20", "-e", CALCULATOR, "-lq", NULL };
const char *spcmd3[] = {MUSIC,    "--name", SPMUSIC, NULL};
static Sp scratchpads[] = {
    /* name          cmd  */
    { SPTERM,      spcmd1},
    { SPCALC,      spcmd2},
    { SPMUSIC,     spcmd3},
};

/* -- Tagging -- */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* class      instance    title             tags mask     isfloating   isterminal   noswallow     monitor */
    { TERMCLASS,  NULL,       NULL,             0,            0,           1,           0,            -1 }, /* every terminal  */
    /* ensure X11 event tester window (xev) don't get swallowed by st; i.e.: opens in a different window */
    { NULL,       NULL,       XEVTITLE,         0,            0,           0,           1,            -1 }, /* xev             */
    /* scratchpads rules */
    { TERMCLASS,  SPTERM,     NULL,             SPTAG(0),     1,           1,           0,            -1 }, /* st scratchpad   */
    { TERMCLASS,  SPCALC,     NULL,             SPTAG(1),     1,           1,           0,            -1 }, /* bc scratchpad   */
    /* pstt scratchpad; set instance to SPMUSIC when the --name flag works */
    { MUSICCLASS, NULL,       NULL,             SPTAG(2),     1,           0,           0,            -1 },
};

/* -- Layout(s) -- */
#include "vanitygaps.c"
static float mfact                  = 0.55;                     /* factor of master area size [0.05..0.95]                          */
static int nmaster                  = 1;                        /* number of clients in master area                                 */
static int resizehints              = 0;                        /* 1 means respect size hints in tiled resizals                     */
static const int lockfullscreen     = 1;                        /* 1 will force focus on the fullscreen window                      */
#define FORCE_VSPLIT 1                                          /* nrowgrid layout: force two clients to always split vertically    */
static const Layout layouts[] = {
    /* symbol                       arrange function                                                                                */
    { "[]=",                        tile                    },  /* Default: Master on left, slaves on right                         */
    { "TTT",                        bstack                  },  /* Master on top, slaves on bottom                                  */

    { "[@]",                        spiral                  },  /* Fibonacci spiral                                                 */
    { "[\\]",                       dwindle                 },  /* Decreasing in size right and leftward                            */

    { "[D]",                        deck                    },  /* Master on left, slaves in monocle-like mode on right             */
    { "[M]",                        monocle                 },  /* All windows on top of eachother                                  */

    { "|M|",                        centeredmaster          },  /* Master in middle, slaves on sides                                */
    { ">M>",                        centeredfloatingmaster  },  /* Same but master floats                                           */

    { "><>",                        NULL                    },  /* no layout function means floating behavior                       */
    { NULL,                         NULL                    },
};

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/* -- Xresources preferences -- */
ResourcePref resources[] = {
    { "color0",                     STRING,         &normbordercolor    },
    { "color8",                     STRING,         &selbordercolor     },
    { "color0",                     STRING,         &normbgcolor        },
    { "color4",                     STRING,         &normfgcolor        },
    { "color0",                     STRING,         &selfgcolor         },
    { "color4",                     STRING,         &selbgcolor         },
    { "borderpx",                   INTEGER,        &borderpx           },
    { "snap",                       INTEGER,        &snap               },
    { "showbar",                    INTEGER,        &showbar            },
    { "topbar",                     INTEGER,        &topbar             },
    { "nmaster",                    INTEGER,        &nmaster            },
    { "resizehints",                INTEGER,        &resizehints        },
    { "mfact",                      FLOAT,          &mfact              },
    { "gappih",                     INTEGER,        &gappih             },
    { "gappiv",                     INTEGER,        &gappiv             },
    { "gappoh",                     INTEGER,        &gappoh             },
    { "gappov",                     INTEGER,        &gappov             },
    { "swallowfloating",            INTEGER,        &swallowfloating    },
    { "smartgaps",                  INTEGER,        &smartgaps          },
};

/* -- Bindings -- */
#include <X11/XF86keysym.h> /* Provides XF86XK_* keys (multimedia keys)  */
#include "shiftview.c"      /* Provides shiftview and shifttag functions */
static const Key keys[] = {
    /* modifier                     key             function            argument */

    /* Move the focus to the next/previous/master window in the stack*/
    { MODKEY,                       XK_j,           focusstack,         {.i = INC(+1) } },
    { MODKEY,                       XK_k,           focusstack,         {.i = INC(-1) } },
    { MODKEY,                       XK_v,           focusstack,         {.i = 0  } },

    /* Move the focused window to the next/previous stack position */
    { MODKEY|ShiftMask,             XK_j,           pushstack,          {.i = INC(+1) } },
    { MODKEY|ShiftMask,             XK_k,           pushstack,          {.i = INC(-1) } },
    { MODKEY|ShiftMask,             XK_v,           pushstack,          {.i = 0  } },

    /* Show only the windows with the selected tag */
    { MODKEY,                       XK_1,           view,               {.ui = 1 << 0} },
    { MODKEY,                       XK_2,           view,               {.ui = 1 << 1} },
    { MODKEY,                       XK_3,           view,               {.ui = 1 << 2} },
    { MODKEY,                       XK_4,           view,               {.ui = 1 << 3} },
    { MODKEY,                       XK_5,           view,               {.ui = 1 << 4} },
    { MODKEY,                       XK_6,           view,               {.ui = 1 << 5} },
    { MODKEY,                       XK_7,           view,               {.ui = 1 << 6} },
    { MODKEY,                       XK_8,           view,               {.ui = 1 << 7} },
    { MODKEY,                       XK_9,           view,               {.ui = 1 << 8} },
    { MODKEY,                       XK_0,           view,               {.ui = ~0    } },
    { MODKEY,                       XK_Tab,         view,               {0           } }, /* previously selected tag */

    /* Replaces the tag for the focused window */
    { MODKEY|ShiftMask,             XK_1,           tag,                {.ui = 1 << 0} },
    { MODKEY|ShiftMask,             XK_2,           tag,                {.ui = 1 << 1} },
    { MODKEY|ShiftMask,             XK_3,           tag,                {.ui = 1 << 2} },
    { MODKEY|ShiftMask,             XK_4,           tag,                {.ui = 1 << 3} },
    { MODKEY|ShiftMask,             XK_5,           tag,                {.ui = 1 << 4} },
    { MODKEY|ShiftMask,             XK_6,           tag,                {.ui = 1 << 5} },
    { MODKEY|ShiftMask,             XK_7,           tag,                {.ui = 1 << 6} },
    { MODKEY|ShiftMask,             XK_8,           tag,                {.ui = 1 << 7} },
    { MODKEY|ShiftMask,             XK_9,           tag,                {.ui = 1 << 8} },
    { MODKEY|ShiftMask,             XK_0,           tag,                {.ui = ~0    } },

    /* Show/hide windows with the selected tag */
    { MODKEY|ControlMask,           XK_1,           toggleview,         {.ui = 1 << 0} },
    { MODKEY|ControlMask,           XK_2,           toggleview,         {.ui = 1 << 1} },
    { MODKEY|ControlMask,           XK_3,           toggleview,         {.ui = 1 << 2} },
    { MODKEY|ControlMask,           XK_4,           toggleview,         {.ui = 1 << 3} },
    { MODKEY|ControlMask,           XK_5,           toggleview,         {.ui = 1 << 4} },
    { MODKEY|ControlMask,           XK_6,           toggleview,         {.ui = 1 << 5} },
    { MODKEY|ControlMask,           XK_7,           toggleview,         {.ui = 1 << 6} },
    { MODKEY|ControlMask,           XK_8,           toggleview,         {.ui = 1 << 7} },
    { MODKEY|ControlMask,           XK_9,           toggleview,         {.ui = 1 << 8} },

    /* Add/remove the selected tag to the focused window */
    { MODKEY|ControlMask|ShiftMask, XK_1,           toggletag,          {.ui = 1 << 0} },
    { MODKEY|ControlMask|ShiftMask, XK_2,           toggletag,          {.ui = 1 << 1} },
    { MODKEY|ControlMask|ShiftMask, XK_3,           toggletag,          {.ui = 1 << 2} },
    { MODKEY|ControlMask|ShiftMask, XK_4,           toggletag,          {.ui = 1 << 3} },
    { MODKEY|ControlMask|ShiftMask, XK_5,           toggletag,          {.ui = 1 << 4} },
    { MODKEY|ControlMask|ShiftMask, XK_6,           toggletag,          {.ui = 1 << 5} },
    { MODKEY|ControlMask|ShiftMask, XK_7,           toggletag,          {.ui = 1 << 6} },
    { MODKEY|ControlMask|ShiftMask, XK_8,           toggletag,          {.ui = 1 << 7} },
    { MODKEY|ControlMask|ShiftMask, XK_9,           toggletag,          {.ui = 1 << 8} },

    /* Show only the windows with the previous/next tag */
    { MODKEY|ShiftMask,             XK_h,           shiftview,          { .i = -1 } },
    { MODKEY|ShiftMask,             XK_l,           shiftview,          { .i = 1  } },

    /* Replace the tag of the focused window the previous/next tag */
    { MODKEY|ControlMask|ShiftMask, XK_h,           shifttag,           { .i = -1 } },
    { MODKEY|ControlMask|ShiftMask, XK_l,           shifttag,           { .i = 1  } },

    /* Move the focused window to the top of the stack (MASTER) */
    { MODKEY,                       XK_Return,      zoom,               {0           } },

    /* Increase the number of masters by 1 */
    { MODKEY,                       XK_o,           incnmaster,         {.i = +1     } },

    /* Decrease the number of masters by 1 */
    { MODKEY|ShiftMask,             XK_o,           incnmaster,         {.i = -1     } },

    /* Kill the focused window */
    { MODKEY|ShiftMask,             XK_BackSpace,   killclient,         {0           } },

    /* Change the current layout */
    { MODKEY,                       XK_t,           setlayout,          {.v = &layouts[0]} }, /* tile */
    { MODKEY|ShiftMask,             XK_t,           setlayout,          {.v = &layouts[1]} }, /* bstack */
    { MODKEY,                       XK_y,           setlayout,          {.v = &layouts[2]} }, /* spiral */
    { MODKEY|ShiftMask,             XK_y,           setlayout,          {.v = &layouts[3]} }, /* dwindle */
    { MODKEY,                       XK_u,           setlayout,          {.v = &layouts[4]} }, /* deck */
    { MODKEY|ShiftMask,             XK_u,           setlayout,          {.v = &layouts[5]} }, /* monocle */
    { MODKEY,                       XK_i,           setlayout,          {.v = &layouts[6]} }, /* centeredmaster */
    { MODKEY|ShiftMask,             XK_i,           setlayout,          {.v = &layouts[7]} }, /* centeredfloatingmaster */
    { MODKEY|ShiftMask,             XK_f,           setlayout,          {.v = &layouts[8]} }, /* floating */

    /* Make the focused window visible across all the tags (aka STICKY) */
    { MODKEY,                       XK_s,           togglesticky,       {0} },

    /* Make the focused window show in fullscreen */
    { MODKEY,                       XK_f,           togglefullscr,      {0} },

    /* Increase/decrease master's size */
    { MODKEY,                       XK_h,           setmfact,           {.f = -0.05} },
    { MODKEY,                       XK_l,           setmfact,           {.f = +0.05} },

    /* Manage gaps */
    { MODKEY,                       XK_a,           togglegaps,         {0         } }, /* Enable/disable gaps */
    { MODKEY|ShiftMask,             XK_a,           defaultgaps,        {0         } }, /* Restore default size */
    { MODKEY,                       XK_z,           incrgaps,           {.i = +3   } }, /* Increase size */
    { MODKEY,                       XK_x,           incrgaps,           {.i = -3   } }, /* Decrease size */
    { MODKEY|ShiftMask,             XK_x,           togglesmartgaps,    {0         } }, /* Enable/disable smart gaps */

    /* Show/hide status bar */
    { MODKEY,                       XK_b,           togglebar,          {0         } },

    /* Handle multiple monitors */
    { MODKEY,                       XK_Left,        focusmon,           {.i = -1   } },
    { MODKEY|ShiftMask,             XK_Left,        tagmon,             {.i = -1   } },
    { MODKEY,                       XK_Right,       focusmon,           {.i = +1   } },
    { MODKEY|ShiftMask,             XK_Right,       tagmon,             {.i = +1   } },

    /* Move a window in/out of the stack (float) */
    { MODKEY|ShiftMask,             XK_space,       togglefloating,     {0         } },

    /* Toggle scratchpads */
    { MODKEY|ShiftMask,             XK_apostrophe,  togglescratch,      {.ui = 1   } }, /* bc */
    { MODKEY|ShiftMask,             XK_exclamdown,  togglescratch,      {.ui = 2   } }, /* psst */
    { MODKEY|ShiftMask,             XK_dead_grave,  togglescratch,      {.ui = 0   } }, /* st */

    /* Launch commands */
    { MODKEY|ShiftMask,             XK_Return,                spawn,              {.v = termcmd} },
    { MODKEY,                       XK_w,                     spawn,              {.v = (const char*[]){ BROWSER, NULL } } },
    { MODKEY,                       XK_e,                     spawn,              {.v = (const char*[]){ EMOJI_PICKER, NULL } } },
    { MODKEY,                       XK_p,                     spawn,              {.v = (const char*[]){ LAUNCHER, NULL } } },
    { MODKEY|Mod1Mask,              XK_s,                     spawn,              SHCMD("scrot -s ~/Pictures/Screenshots/%b%d::%H%M%S.png")},
    { 0,                            XF86XK_MonBrightnessUp,   spawn,              SHCMD("brightnessctl -q set 5%+")},
    { 0,                            XF86XK_MonBrightnessDown, spawn,              SHCMD("brightnessctl -q set 5%-")},
    { 0,                            XF86XK_AudioRaiseVolume,  spawn,              SHCMD("wpctl set-volume @DEFAULT_SINK@ 5%+")},
    { 0,                            XF86XK_AudioLowerVolume,  spawn,              SHCMD("wpctl set-volume @DEFAULT_SINK@ 5%-")},
    { 0,                            XF86XK_AudioMute,         spawn,              SHCMD("wpctl set-mute @DEFAULT_SINK@ toggle")},

    /* Load X resources */
    { MODKEY,                       XK_F5,          xrdb,               {.v = NULL } },

    /* End X session */
    { MODKEY|ShiftMask,             XK_q,           quit,               {0         } },
};

/*  -- Mouse definitions --                      */

/*  Button1          -   left click              */
/*  Button2          -   middle click            */
/*  Button3          -   right click             */
/*  Button4          -   scroll up               */
/*  Button5          -   scroll down             */

/*  ClkTagBar        -   top left corner         */
/*  ClkStatusText    -   top right corner        */
/*  ClkClientWin     -   focused window          */
/*  ClkRootWin       -   background              */
/*  ClkLtSymbol      -   layout symbol           */
/*  ClkWinTitle      -   focused window title    */

static const Button buttons[] = {
    /* click                event mask      button          function        argument */

    /* Mod + hold left/right click to move/resize a window making it float */
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0      } },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0      } },

    /* Mod + scroll up/down to increase/decrease gap's size */
    { ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
    { ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },

    /* Mod + middle click restore default gap's size */
    { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0      } },

    /* Left click on tag switch current view to clicked tag */
    { ClkTagBar,            0,              Button1,        view,           {0      } },

    /* Mod + left click on tag move the current window to the clicked tag */
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0      } },

    /* Right click on the tag to add/remove the clicked tag from the current view */
    { ClkTagBar,            0,              Button3,        toggleview,     {0      } },

    /* Mod + right click on the tag to add/remove the ckicked tag to the focused window */
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0      } },

    /* Middle click on the background show/hide the bar */
    { ClkRootWin,           0,              Button2,        togglebar,      {0      } },

    /* Scroll up/down over the tags bar set the current view to the previous/next tag */
    { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
    { ClkTagBar,            0,              Button5,        shiftview,      {.i = 1 } },

};

