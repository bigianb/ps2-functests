
TGTDIR := build
DIRS = src

.PHONY: all clean

all: $(DIRS) | $(TGTDIR)

.PHONY: $(DIRS)
$(DIRS): | $(TGTDIR)
	$(MAKE) -C $@ all	

$(TGTDIR):
	mkdir -p $(TGTDIR)

clean:
	rm -f -r *.o $(TGTDIR)
	for i in $(DIRS); do \
		$(MAKE) -C $$i clean; \
	done; 
