## Process this file with automake to produce Makefile.in

SUBDIRS = src po doc

EXTRA_DIST = \
	autogen.sh \
	gnome-ding.glade \
	gnome-ding.gladep \
	gnome-ding.spec.in \
	gnome-ding.desktop \
	thesaurus-de.ding \
	thesaurus-es.ding \
	lat-de.ding \
	de-en.ding \
	en-es.ding \
	en-fr.ding \
	en-no.ding \
	no-en.ding

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	    fi \
	  done \
	fi; \
	if test -f $(srcdir)/pixmaps/gnome-ding.png; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/; \
	  $(INSTALL_DATA) $(srcdir)/pixmaps/gnome-ding.png $(DESTDIR)$(datadir)/pixmaps/; \
	fi; \
	if [ -f $(srcdir)/de-en.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) de-en.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/thesaurus-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) thesaurus-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/thesaurus-es.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) thesaurus-es.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/lat-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) lat-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-es.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-es.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-fr.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-fr.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-no.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-no.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/no-en.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) no-en.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/gnome-ding.desktop ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/applications/; \
	  $(INSTALL_DATA) gnome-ding.desktop $(DESTDIR)/usr/share/applications/; \
	fi;

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

