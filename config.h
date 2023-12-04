#define COLOR(hex) \
    { ((hex >> 24) & 0xFF) / 255.0f, ((hex >> 16) & 0xFF) / 255.0f, ((hex >> 8) & 0xFF) / 255.0f, (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 0;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 1;  /* 1 means no outer gap when there is only one window */
static const int monoclegaps               = 0;  /* 1 means outer gaps in monocle layout */
static const unsigned int borderpx         = 2;  /* border pixel of windows */
static const unsigned int gappih           = 10; /* horiz inner gap between windows */
static const unsigned int gappiv           = 10; /* vert inner gap between windows */
static const unsigned int gappoh           = 10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov           = 0; /* vert outer gap between windows and screen edge */
static const float bordercolor[]           = COLOR(0x666666ff);
static const float focuscolor[]            = COLOR(0xffd5a0ff);
static const float urgentcolor[]           = COLOR(0xea6962ff);
/* To conform the xdg-protocol, set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = COLOR(0x111111ff); 

/* Autostart */
static const char *const autostart[] = {
        "/bin/sh", "-c", "$HOME/.dwlrc", NULL,
        NULL /* terminate */
};

/* tagging - tagcount must be no greater than 31 */
#define TAGCOUNT (9)
static const int tagcount = 9;

/* logging */
static int log_level = WLR_ERROR;

static const Rule rules[] = {
	/* app_id     title                                          tags mask     isfloating   monitor */
	/* examples:
	{ "Gimp",     NULL,                                          0,            1,           -1 },
	*/
   { "discord", NULL,                                          1 << 7,       0,           -1 },
   { "firefox", NULL,                                          1 << 8,       0,           -1 },
   { "firefox", "Library",                                     0,            1,           -1 },
   { "firefox", "Firefox - Sharing Indicator",                 0,            1,           -1 },
   { "firefox", "Extension: (Tree Style Tab) - Close tabs?",   0,            1,           -1 },
   { "waterfox", NULL,                                         1 << 8,       0,           -1 },
   { "waterfox", "Library",                                    0,            1,           -1 },
   { "waterfox", "Waterfox - Sharing Indicator",               0,            1,           -1 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[tile]",      tile },
	{ "[float]",      NULL },    /* no layout function means floating behavior */
	{ "[monocle]",      monocle },
};

/* monitors */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55, 1,      1.3,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, -1,  -1 }
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = NULL,
	.layout = "be",
};

static const int repeat_rate = 25;
static const int repeat_delay = 300;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;
/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(SKEY,KEY,TAG) \
	{ MODKEY,                    KEY,             view,           {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,            toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_ALT,   KEY,             tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_ALT|WLR_MODIFIER_SHIFT,SKEY,toggletag,  {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define VOLUME(vol) { "/bin/sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ " #vol "", NULL}
#define SOURCE_VOLUME(vol) { "/bin/sh", "-c", "pactl set-source-volume @DEFAULT_SOURCE@ " #vol "", NULL}

/* commands */
static const char *volupcmd[]   = VOLUME("+1%");
static const char *voldowncmd[] = VOLUME("-1%");
static const char *sourceupcmd[]   = SOURCE_VOLUME("+1%");
static const char *sourcedowncmd[] = SOURCE_VOLUME("-1%");
static const char *termcmd[] = { "foot", NULL };
static const char *menucmd[] = { "sysmenu", NULL };
static const char *passtypecmd[] = { "passbemenu", "type", NULL };
static const char *passclipcmd[] = { "passbemenu", "clip", NULL };
static const char *screenshotcmd[] = { "screenshot", NULL };
static const char *noticmd[] = { "fnottmgr", NULL };

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ 0,            XKB_KEY_XF86AudioRaiseVolume, spawn,        {.v = volupcmd  } },
	{ 0,            XKB_KEY_XF86AudioLowerVolume, spawn,        {.v = voldowncmd} },
	{ WLR_MODIFIER_SHIFT,        XKB_KEY_XF86AudioRaiseVolume, spawn,{.v = sourceupcmd  } },
	{ WLR_MODIFIER_SHIFT,        XKB_KEY_XF86AudioLowerVolume, spawn,{.v = sourcedowncmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_S,          spawn,          {.v = screenshotcmd} },
	{ MODKEY,                    XKB_KEY_f,          spawn,          {.v = noticmd} },
	{ MODKEY,                    XKB_KEY_p,          spawn,          {.v = passtypecmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_P,          spawn,          {.v = passclipcmd} },
	{ MODKEY,                    XKB_KEY_Return,     spawn,          {.v = menucmd} },
	{ MODKEY,                    XKB_KEY_s,          spawn,          {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_n,          focusstack,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_N,          focusstack,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,          focusdir,       {.ui = 0} },
	{ MODKEY,                    XKB_KEY_l,          focusdir,       {.ui = 1} },
	{ MODKEY,                    XKB_KEY_k,          focusdir,       {.ui = 2} },
	{ MODKEY,                    XKB_KEY_j,          focusdir,       {.ui = 3} },
	{ MODKEY,                    XKB_KEY_i,          incnmaster,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_d,          incnmaster,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_equal,      setmfact,       {.f = 1} },
	{ MODKEY,                    XKB_KEY_minus,      setmfact,       {.f = -0.05} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_plus,       setmfact,       {.f = +0.05} },
	{ MODKEY,                    XKB_KEY_space,      zoom,           {0} },
	{ MODKEY,                    XKB_KEY_agrave,     view,           {0} },
	{ MODKEY,                    XKB_KEY_q,          killclient,     {0} },
	{ MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,                    XKB_KEY_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	//{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
	//{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating, {0} },
	//{ MODKEY,                    XKB_KEY_e,          togglefullscreen,{0} },
	{ MODKEY,                    XKB_KEY_agrave,     view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_agrave,     tag,            {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_ampersand,                  0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_eacute,                     1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_quotedbl,                   2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_apostrophe,                 3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_parenleft,                  4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_section,                    5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_egrave,                     6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_exclam,                     7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_ccedilla,                   8),
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Q,          quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),

};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
