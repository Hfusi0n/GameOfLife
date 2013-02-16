#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_
// multi-language support
// TODO: add proper gettext support
#define gettext(STR) STR
#define _(STR) gettext(STR)

#endif
