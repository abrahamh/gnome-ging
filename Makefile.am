## Process this file with automake to produce Makefile.in

SUBDIRS = src po doc

EXTRA_DIST = \
	autogen.sh \
	gnome-ding.glade \
	gnome-ding.gladep \
	gnome-ding.spec.in \
	gnome-ding.desktop \
	debian \
	thesaurus-de.ding \
	thesaurus-es.ding \
	lat-de.ding \
	lat-en.ding \
	en-lat.ding \
	de-en.ding \
	de-fr.ding \
	de-it.ding \
	de-nl.ding \
	de-pt.ding \
	en-es.ding \
	en-de.ding \
	en-fr.ding \
	en-sv.ding \
	en-pt.ding \
	en-it.ding \
	en-no.ding \
	en-ar.ding \
	es-de.ding \
	fr-de.ding \
	it-de.ding \
	it-en.ding \
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
	if [ -f $(srcdir)/de-fr.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) de-fr.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/de-it.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) de-it.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/de-pt.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) de-pt.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/de-nl.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) de-nl.ding $(DESTDIR)/usr/share/dict; \
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
	if [ -f $(srcdir)/lat-en.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) lat-en.ding $(DESTDIR)/usr/share/dict; \
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
	if [ -f $(srcdir)/en-pt.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-pt.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/no-en.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) no-en.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/fr-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) fr-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/es-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) es-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-lat.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-lat.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-sv.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-sv.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-it.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-it.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/en-ar.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) en-ar.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/it-de.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) it-de.ding $(DESTDIR)/usr/share/dict; \
	fi; \
	if [ -f $(srcdir)/it-en.ding ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) it-en.ding $(DESTDIR)/usr/share/dict; \
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

