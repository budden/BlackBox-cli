MODULE Console;

	(*
		A. V. Shiryaev, 2012.10

		Interface based on OpenBUGS Console
	*)

	TYPE
		Console* = POINTER TO ABSTRACT RECORD END;

	VAR
		cons: Console;

	(* Console *)

	PROCEDURE (c: Console) WriteStr- (IN s: ARRAY OF CHAR), NEW, ABSTRACT;
	PROCEDURE (c: Console) WriteChar- (ch: CHAR), NEW, ABSTRACT;
	PROCEDURE (c: Console) WriteLn-, NEW, ABSTRACT;

	(*
		post:
			s = "": end of input or input error
			s # "": line with end of line postfix
	*)
	PROCEDURE (c: Console) ReadLn- (OUT s: ARRAY OF CHAR), NEW, ABSTRACT;


	PROCEDURE WriteStr* (IN text: ARRAY OF CHAR);
	BEGIN
		cons.WriteStr(text)
	END WriteStr;

	PROCEDURE WriteChar* (c: CHAR);
	BEGIN
		cons.WriteChar(c)
	END WriteChar;

	PROCEDURE WriteLn*;
	BEGIN
		cons.WriteLn
	END WriteLn;

	PROCEDURE ReadLn* (OUT text: ARRAY OF CHAR);
	BEGIN
		cons.ReadLn(text)
	END ReadLn;


	PROCEDURE SetConsole* (c: Console);
	BEGIN
		cons := c
	END SetConsole;

END Console.
