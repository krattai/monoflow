#ifndef INTL_H
#define INTL_H

#include <glib.h>
#ifdef GNOME
#  include <libgnome/gnome-i18n.h>
#else
#  ifdef ENABLE_NLS
#    include <libintl.h>
#    define _(String) gettext(String)
#    ifdef gettext_noop
#      define N_(String) gettext_noop(String)
#    else
#      define N_(String) (String)
#    endif
#  else /* NLS is disabled */
#    define _(String) (String)
#    define N_(String) (String)
#    define gettext(String) (String)
#    define textdomain(Domain)
#    define bindtextdomain(Package, Directory)
#  endif
#endif

const GList *intl_get_language_list (void);
int    intl_score_locale      (const gchar *locale);

#endif
