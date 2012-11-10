MODULE HostDialog;

	(* for StdLog and Dialog.SetShowHook *)

	IMPORT Dialog, Console;

	TYPE
		ShowHook = POINTER TO RECORD (Dialog.ShowHook) END;

	PROCEDURE ShowParamMsg* (IN str, p0, p1, p2: ARRAY OF CHAR);
		VAR st: ARRAY 512 OF CHAR;
	BEGIN
		ASSERT(str # "", 20);
(*
		IF Dialog.showsStatus THEN
			ShowParamStatus(str, p0, p1, p2)
		ELSE
*)
			Dialog.MapParamString(str, p0, p1, p2, st);
			Console.WriteStr(st); Console.WriteLn
(*
		END
*)
	END ShowParamMsg;

	PROCEDURE ShowParamStatus* (IN str, p0, p1, p2: ARRAY OF CHAR);
		VAR st: ARRAY 513 OF CHAR;
	BEGIN
		Dialog.MapParamString(str, p0, p1, p2, st);
		Console.WriteStr(st); Console.WriteLn
	END ShowParamStatus;

	PROCEDURE (h: ShowHook) ShowParamMsg (IN str, p0, p1, p2: ARRAY OF CHAR);
	BEGIN
		ShowParamMsg(str, p0, p1, p2)
	END ShowParamMsg;

	PROCEDURE (h: ShowHook) ShowParamStatus (IN str, p0, p1, p2: ARRAY OF CHAR);
	BEGIN
		ShowParamStatus(str, p0, p1, p2)
	END ShowParamStatus;

	PROCEDURE Init;
		VAR
			showHook: ShowHook;
	BEGIN
		NEW(showHook); Dialog.SetShowHook(showHook)
	END Init;

BEGIN
	Init
END HostDialog.
