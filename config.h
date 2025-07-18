/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx  = 3;      /* border pixel of windows */
static unsigned int snap      = 32;     /* snap pixel */
static unsigned int gappih    = 8;      /* horiz inner gap between windows */
static unsigned int gappiv    = 8;      /* vert inner gap between windows */
static unsigned int gappoh    = 10;     /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 10;     /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;      /* 1 means swallow floating windows by default */
static int smartgaps          = 0;      /* 1 means no outer gap when there is only one window */
static int showbar            = 1;      /* 0 means no bar */
static int topbar             = 1;      /* 0 means bottom bar */
static char *fonts[]          = {
    "monospace:size=12",
    "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
/* #ffb86c #ff79c6 #13b3b3 #ffb3b3 */
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#ffe6e6";
static char selfgcolor[]            = "#4d4d4d";
static char selbordercolor[]        = "#ff79c6";
static char selbgcolor[]            = "#13b3b3";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
const char *spcmd3[] = {TERMINAL, "-n", "spbigterm", "-g", "177x46", NULL };
const char *spcmd4[] = {TERMINAL, "-n", "today", "-g", "147x49", "-e", "today",NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spcalc",      spcmd2},
    {"spbigterm",   spcmd3},
    {"today",       spcmd4},
};

/* tagging */
static const char *tags[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class        instance        title            tags mask   isfloating   isterminal  noswallow  monitor */
    { "Gimp",       NULL,           NULL,            1 << 8,       0,           0,         0,        -1 },
    { TERMCLASS,    NULL,           NULL,            0,            0,           1,         0,        -1 },
    { NULL,         NULL,           "Event Tester",  0,            0,           0,         1,        -1 },
    { TERMCLASS,    "floatterm",    NULL,            0,            1,           1,         0,        -1 },
    { TERMCLASS,    "bg",           NULL,            1 << 7,       0,           1,         0,        -1 },
    { TERMCLASS,    "spterm",       NULL,            SPTAG(0),     1,           1,         0,        -1 },
    { TERMCLASS,    "spcalc",       NULL,            SPTAG(1),     1,           1,         0,        -1 },
    { TERMCLASS,    "spbigterm",    NULL,            SPTAG(2),     1,           1,         0,        -1 },
    { TERMCLASS,    "today",        NULL,            SPTAG(3),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.50;  /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;     /* number of clients in master area */
static int resizehints = 0;     /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1          /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol   arrange function */
    { "[]=",    tile },         /* Default: Master on left, slaves on right */
    { "TTT",    bstack },       /* Master on top, slaves on bottom */
    { "[@]",    spiral },       /* Fibonacci spiral */
    { "[\\]",   dwindle },      /* Decreasing in size right and leftward */
    { "[D]",    deck },         /* Master on left, slaves in monocle-like mode on right */
    { "[M]",    monocle },      /* All windows on top of eachother */
    { "|M|",    centeredmaster },/* Master in middle, slaves on sides */
    { ">M>",    centeredfloatingmaster }, /* Same but master floats */
    { "><>",    NULL },         /* no layout function means floating behavior */
    { NULL,     NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
{ MOD,  XK_s, ACTION##stack,  {.i = INC(+1) } }, \
{ MOD,  XK_n, ACTION##stack,  {.i = INC(-1) } }, \
{ MOD,  XK_c,   ACTION##stack,  {.i = 0 } }, \
/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "color0",         STRING, &normbordercolor },
    { "color8",         STRING, &selbordercolor },
    { "color0",         STRING, &normbgcolor },
    { "color4",         STRING, &normfgcolor },
    { "color0",         STRING, &selfgcolor },
    { "color4",         STRING, &selbgcolor },
    { "borderpx",       INTEGER, &borderpx },
    { "snap",           INTEGER, &snap },
    { "showbar",        INTEGER, &showbar },
    { "topbar",         INTEGER, &topbar },
    { "nmaster",        INTEGER, &nmaster },
    { "resizehints",    INTEGER, &resizehints },
    { "mfact",          FLOAT,   &mfact },
    { "gappih",         INTEGER, &gappih },
    { "gappiv",         INTEGER, &gappiv },
    { "gappoh",         INTEGER, &gappoh },
    { "gappov",         INTEGER, &gappov },
    { "swallowfloating",INTEGER, &swallowfloating },
    { "smartgaps",      INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
    /* modifier         key         function        argument */
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
    TAGKEYS(            XK_twosuperior, 0)
    TAGKEYS(            XK_1,           1)
    TAGKEYS(            XK_2,           2)
    TAGKEYS(            XK_3,           3)
    TAGKEYS(            XK_4,           4)
    TAGKEYS(            XK_5,           5)
    TAGKEYS(            XK_6,           6)
    TAGKEYS(            XK_7,           7)
    TAGKEYS(            XK_8,           8)
    TAGKEYS(            XK_9,           9)
    TAGKEYS(            XK_0,           10)



    { MODKEY,           XK_a,           killclient,     {0} },
    { MODKEY,           XK_b,           togglebar,      {0} },
    { MODKEY,           XK_e,           setlayout,      {.v = &layouts[5]} }, /* monocle */
    { MODKEY,           XK_f,           togglefullscr,  {0} },
    { MODKEY,           XK_g,           togglegaps,     {0} },
    { MODKEY|ShiftMask, XK_g,           defaultgaps,    {0} },
    { MODKEY,           XK_h,           setlayout,      {.v = &layouts[4]} }, /* deck */
    { MODKEY|ShiftMask, XK_i,           togglesticky,   {0} },
    { MODKEY,           XK_k,           togglescratch,  {.ui = 1} },
    { MODKEY,           XK_l,           togglescratch,  {.ui = 3} },
    { MODKEY|ShiftMask, XK_l,           spawn,          SHCMD("newnote")},
    { MODKEY,           XK_m,           togglescratch,  {.ui = 2} },
    { MODKEY,           XK_o,           togglescratch,  {.ui = 0} },
    { MODKEY,           XK_t,           setlayout,      {.v = &layouts[2]} }, /* spiral */
    { MODKEY|ShiftMask, XK_t,           setlayout,      {.v = &layouts[1]} }, /* bstack */
    { MODKEY,           XK_u,           setmfact,       {.f = -0.05} },
    { MODKEY|ShiftMask, XK_u,           setmfact,       {.f = +0.05} },
    { MODKEY,           XK_w,           spawn,          {.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } },
    { MODKEY,           XK_z,           incrgaps,       {.i = +3 } },
    { MODKEY|ShiftMask, XK_z,           incrgaps,       {.i = -3 } },
    { MODKEY,           XK_comma,       spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
    { MODKEY,           XK_colon,       spawn,          {.v = (const char*[]){ "dmenu_run", NULL } } },
    { MODKEY,           XK_Tab,         view,           {0} },
    { MODKEY,           XK_Return,      spawn,          {.v = termcmd } },
    { MODKEY|ShiftMask, XK_Return,      spawn,          {.v = (const char*[]){ "sd", NULL } } }, /* spawn terminal in same directory as current*/
    { MODKEY,           XK_space,       zoom,           {0} },
    { MODKEY,           XK_BackSpace,   NULL,           {0} },
    { MODKEY|ShiftMask, XK_BackSpace,   spawn,          {.v = (const char*[]){ "sysact", NULL } } },

    /* Null bindings for unused letters and their Shift versions */
    { MODKEY|Mod5Mask,  XK_c,           spawn,          SHCMD("notify-send '$ is binded!!!'") },
    { MODKEY,           XK_d,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_d,           NULL,           {0} },
    { MODKEY,           XK_j,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_j,           NULL,           {0} },
    { MODKEY,           XK_p,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_p,           NULL,           {0} },
    { MODKEY,           XK_q,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_q,           NULL,           {0} },
    { MODKEY,           XK_r,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_r,           NULL,           {0} },
    { MODKEY,           XK_v,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_v,           NULL,           {0} },
    { MODKEY,           XK_x,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_x,           NULL,           {0} },
    { MODKEY,           XK_y,           NULL,           {0} },
    { MODKEY|ShiftMask, XK_y,           NULL,           {0} },
    { MODKEY,           XK_semicolon,   NULL,           {0} },
    { MODKEY|ShiftMask, XK_semicolon,   NULL,           {0} },
    { MODKEY,           XK_minus,       NULL,           {0} },
    { MODKEY,           XK_question,    NULL,           {0} },
    { MODKEY,           XK_apostrophe,  NULL,           {0} },
    { MODKEY,           XK_percent,     NULL,           {0} },
    { MODKEY,           XK_equal,       NULL,           {0} },
    { MODKEY,           XK_colon,       NULL,           {0} },
    { MODKEY|ShiftMask, XK_colon,       NULL,           {0} },
    { MODKEY,           XK_bar,         NULL,           {0} },
    { MODKEY,           XK_period,      NULL,           {0} },
    { MODKEY,           XK_exclam,      NULL,           {0} },

    { 0, XF86XK_AudioMute,              spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume,       spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume,       spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioMicMute,           spawn,          SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
    { 0, XF86XK_MonBrightnessUp,        spawn,          SHCMD("brightnessctl -q s +15") },
    { 0, XF86XK_MonBrightnessDown,      spawn,          SHCMD("brightnessctl -q s 15-") },
    { 0, XF86XK_ScreenSaver,            spawn,          SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
    { 0, XF86XK_Sleep,                  spawn,          {.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
#ifndef __OpenBSD__
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
    { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
    { ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
    { ClkTagBar,            0,              Button5,        shiftview,      {.i = 1} },
    { ClkRootWin,           0,              Button2,        togglebar,      {0} },
};
