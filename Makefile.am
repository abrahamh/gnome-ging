## Process this file with automake to produce Makefile.in

SUBDIRS = src po doc

EXTRA_DIST = \
	autogen.sh \
	gnome-ding.glade \
	gnome-ding.gladep \
	gnome-ding.spec \
	gnome-ding.desktop \
	ger-eng.ding \
	thesaurus-de.ding \
	lat-deu.ding

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
	if [ -f $(srcdir)/ger-eng.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) ger-eng.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/thesaurus-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) thesaurus-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/lat-deu.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) lat-deu.ding $(DESTDIR)/usr/share/dict; \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

