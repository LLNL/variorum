SHELL=/bin/sh
CLASS=W
VERSION=

default: ep

EP: ep
ep:
	cd EP; $(MAKE) CLASS=$(CLASS)

# It would be nice to make clean in each subdirectory (the targets
# are defined) but on a really clean system this won't work
# because those makefiles need config/make.def
clean:
	- rm -f *~ */*~ */*.o */npbparams.h */*.obj */*.exe
	- rm -f sys/setparams sys/setparams.h

veryclean: clean
	- rm -f config/make.def
	- rm -f bin/ep.*
