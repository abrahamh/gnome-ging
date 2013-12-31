## Process this file with automake to produce Makefile.in

SUBDIRS = src po doc debian

EXTRA_DIST = \
	autogen.sh \
	gnome-ding.glade \
	gnome-ding.gladep \
	gnome-ding.spec.in \
	gnome-ding.desktop 


DING_DICT = \
	de-en.ding \
	en-de.ding \
	en-ar.ding


DING_THES = \
	thesaurus_de.ding 

DING_DOC = \
	AUTHORS \
	COPYING \
	INSTALL \
	README

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
	for P in ${DING_THES} ; do \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA)  $${P} $(DESTDIR)/usr/share/dict; \
	done ;\
	for P in ${DING_DICT} ; do \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/dict/; \
	  $(INSTALL_DATA) $${P} $(DESTDIR)/usr/share/dict; \
	done ;\
	for P in ${DING_DOC} ; do \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/doc/gnome-ding/; \
	  $(INSTALL_DATA)  $${P} $(DESTDIR)/usr/share/doc/gnome-ding; \
	done ;\
	if [ -f $(srcdir)/gnome-ding.desktop ] ; then \
	  $(mkinstalldirs) $(DESTDIR)/usr/share/applications/; \
	  $(INSTALL_DATA) gnome-ding.desktop $(DESTDIR)/usr/share/applications/; \
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
	tar -cpj \
		${DING_THES} \
		${DING_DICT} \
	-f $(distdir)-dict.tar.bz2 

clean:
	rm -f *~
