
CLANG_FORMAT = $(type -p clang-format10)

all: libnygma libriot ny

libnygma:
	b -v libnygma/

libriot:
	b -v libriot

ny:
	b -v nygma/

format:
	find $in \( -name '*.hxx' -or -name '*.cxx' \) -exec $CLANG_FORMAT --verbose -style=file -i '{}' +
