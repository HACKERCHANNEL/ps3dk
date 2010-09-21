
samplesdir = @PS3SDK@/samples

SAMPLES = \
        template/elf_template \
	threads/busywait \
	threads/mutex \
	network/lwip_testbed

all:

# Install all of the samples.  Rename any files that end in a .sample extension.
SAMPLES_INSTALL = $(INSTALL_DATA)
install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(samplesdir)
	@list='$(SAMPLES)'; for s in $$list; do \
	  $(mkinstalldirs) $(DESTDIR)$(samplesdir)/$$s; \
	  for p in $(srcdir)/$$s/*; do \
	    if test -f $$p; then \
	      f="`echo $$p | sed -e 's|^.*/||'`"; \
	      echo " $(SAMPLES_INSTALL) $$p $(DESTDIR)$(samplesdir)/$$s/$$f"; \
	      $(SAMPLES_INSTALL) $$p $(DESTDIR)$(samplesdir)/$$s/$$f; \
	    else :; fi; \
	  done; \
	done
	@for p in `find $(DESTDIR)$(samplesdir) -name *.sample -print `; do \
	  f="`echo $$p | sed -e 's/\.sample$$//'`"; \
	  echo " mv $$p $$f"; \
	  mv $$p $$f;  \
	done

uninstall-hook:
	@list='$(SAMPLES)'; for s in $$list; do \
	  for p in $(DESTDIR)$(samplesdir)/$$s/*; do \
	    if test -f $$p; then \
	      p="`echo $$p | sed -e 's|^.*/||'`"; \
	      echo " rm -f $(DESTDIR)$(samplesdir)/$$s/$$p"; \
	      rm -f $(DESTDIR)$(samplesdir)/$$s/$$p; \
	    else :; fi; \
	  done; \
	done

EXTRA_DIST = $(SAMPLES)
