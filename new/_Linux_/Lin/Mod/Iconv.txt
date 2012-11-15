MODULE LinIconv ["libc.so.6"];

	IMPORT Libc := LinLibc;

	TYPE
		PtrVoid = Libc.PtrVoid;
		PtrSTR* = Libc.PtrSTR;
		PtrLSTR* = POINTER TO ARRAY [untagged] OF CHAR;
		size_t* = Libc.size_t;

		iconv_t* = PtrVoid;

	PROCEDURE [ccall] iconv_open* (tocode, fromcode: PtrSTR): iconv_t;
	PROCEDURE [ccall] iconv_close* (cd: iconv_t): INTEGER;

	PROCEDURE [ccall] iconv* (cd: iconv_t; VAR [nil] inbuf: PtrSTR; VAR inbytesleft: size_t; VAR [nil] outbuf: PtrSTR; VAR outbytesleft: size_t): size_t;

	PROCEDURE [ccall] iconv_encode* ["iconv"] (cd: iconv_t; VAR [nil] inbuf: PtrLSTR; VAR inbytesleft: size_t; VAR [nil] outbuf: PtrSTR; VAR outbytesleft: size_t): size_t;

	PROCEDURE [ccall] iconv_decode* ["iconv"] (cd: iconv_t; VAR [nil] inbuf: PtrSTR; VAR inbytesleft: size_t; VAR [nil] outbuf: PtrLSTR; VAR outbytesleft: size_t): size_t;

END LinIconv.
