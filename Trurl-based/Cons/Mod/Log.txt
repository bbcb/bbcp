MODULE ConsLog;

	(*
		A. V. Shiryaev, 2012.10

		Log.Hook implementation
		based on StdLog
	*)

	IMPORT
		Log, Views, Dialog,
		TextModels, TextMappers,
		Console;

	TYPE
		LogHook = POINTER TO RECORD (Log.Hook) END;

	VAR
		logAlerts: BOOLEAN;
		subOut: TextMappers.Formatter;

		buf: TextModels.Model;
		textR: TextModels.Reader;

	(* Sub support *)

	PROCEDURE* Guard (o: ANYPTR): BOOLEAN;
	BEGIN
		RETURN o # NIL
	END Guard;

	PROCEDURE* ClearBuf;
		VAR subBuf: TextModels.Model;
	BEGIN
		subBuf := subOut.rider.Base(); subBuf.Delete(0, subBuf.Length())
	END ClearBuf;

	PROCEDURE* FlushBuf;
		VAR c: CHAR;
	BEGIN
		IF buf.Length() > 0 THEN
			textR.SetPos(0);
			textR.ReadChar(c);
			WHILE ~textR.eot DO
				IF c = 0DX THEN
					Console.WriteLn
				ELSE
					Console.WriteChar(c)
				END;
				textR.ReadChar(c)
			END;
			buf.Delete(0, buf.Length())
		END
	END FlushBuf;

	PROCEDURE* SubFlush;
	BEGIN
		IF Log.synch THEN
			FlushBuf;
			(* IF Log.force THEN Views.RestoreDomain(text.Domain()) END *)
		END;
	END SubFlush;

	PROCEDURE (log: LogHook) Guard* (o: ANYPTR): BOOLEAN;
	BEGIN RETURN Guard(o)
	END Guard;

	PROCEDURE (log: LogHook) ClearBuf*;
	BEGIN ClearBuf
	END ClearBuf;

	PROCEDURE (log: LogHook) FlushBuf*;
	BEGIN FlushBuf
	END FlushBuf;

	PROCEDURE (log: LogHook) Beep*;
	BEGIN Dialog.Beep
	END Beep;

	PROCEDURE (log: LogHook) Char* (ch: CHAR);
	BEGIN
		subOut.WriteChar(ch); SubFlush
	END Char;

	PROCEDURE (log: LogHook) Int* (n: INTEGER);
	BEGIN
		subOut.WriteChar(" "); subOut.WriteInt(n); SubFlush
	END Int;

	PROCEDURE (log: LogHook) Real* (x: REAL);
	BEGIN
		subOut.WriteChar(" "); subOut.WriteReal(x); SubFlush
	END Real;

	PROCEDURE (log: LogHook) String* (IN str: ARRAY OF CHAR);
	BEGIN
		subOut.WriteString(str); SubFlush
	END String;

	PROCEDURE (log: LogHook) Bool* (x: BOOLEAN);
	BEGIN
		subOut.WriteChar(" "); subOut.WriteBool(x); SubFlush
	END Bool;

	PROCEDURE (log: LogHook) Set* (x: SET);
	BEGIN
		subOut.WriteChar(" "); subOut.WriteSet(x); SubFlush
	END Set;

	PROCEDURE (log: LogHook) IntForm* (x: INTEGER; base, minWidth: INTEGER; fillCh: CHAR; showBase: BOOLEAN);
	BEGIN
		subOut.WriteIntForm(x, base, minWidth, fillCh, showBase); SubFlush
	END IntForm;

	PROCEDURE (log: LogHook) RealForm* (x: REAL; precision, minW, expW: INTEGER; fillCh: CHAR);
	BEGIN
		subOut.WriteRealForm(x, precision, minW, expW, fillCh); SubFlush
	END RealForm;

	PROCEDURE (log: LogHook) Tab*;
	BEGIN
		subOut.WriteTab; SubFlush
	END Tab;

	PROCEDURE (log: LogHook) Ln*;
	BEGIN
		subOut.WriteLn; SubFlush;
		(* IF Log.synch THEN Views.RestoreDomain(text.Domain()) END *)
	END Ln;

	PROCEDURE (log: LogHook) Para*;
	BEGIN
		subOut.WritePara; SubFlush;
		(* IF Log.synch THEN Views.RestoreDomain(text.Domain()) END *)
	END Para;

	PROCEDURE (log: LogHook) View* (v: ANYPTR);
	BEGIN
		IF (v # NIL) & (v IS Views.View) THEN
			subOut.WriteView(v(Views.View)); SubFlush
		END
	END View;

	PROCEDURE (log: LogHook) ViewForm* (v: ANYPTR; w, h: INTEGER);
	BEGIN
		ASSERT(v # NIL, 20);
		IF (v # NIL) & (v IS Views.View) THEN
			subOut.WriteViewForm(v(Views.View), w, h); SubFlush
		END
	END ViewForm;

	PROCEDURE (log: LogHook) ParamMsg* (IN s, p0, p1, p2: ARRAY OF CHAR);
		VAR msg: ARRAY 256 OF CHAR; i: INTEGER; ch: CHAR;
	BEGIN
		IF logAlerts THEN
			(* IF Log.synch THEN Open END; *)
			Dialog.MapParamString(s, p0, p1, p2, msg);
			i := 0; ch := msg[0];
			WHILE ch # 0X DO
				IF ch = TextModels.line THEN subOut.WriteLn
				ELSIF ch = TextModels.para THEN subOut.WritePara
				ELSIF ch = TextModels.tab THEN subOut.WriteTab
				ELSIF ch >= " " THEN subOut.WriteChar(ch)
				END;
				INC(i); ch := msg[i];
			END;
			subOut.WriteLn; SubFlush
		(* ELSE
			HostDialog.ShowParamMsg(s, p0, p1, p2) *)
		END
	END ParamMsg;


	PROCEDURE AttachSubLog;
		VAR h: LogHook;
	BEGIN
		subOut.ConnectTo(TextModels.dir.New());
		buf := subOut.rider.Base();
		textR := buf.NewReader(NIL);
		NEW(h);
		Log.SetHook(h);
	END AttachSubLog;

	PROCEDURE DetachSubLog;
	BEGIN
		Log.SetHook(NIL)
	END DetachSubLog;

BEGIN
	AttachSubLog
CLOSE
	DetachSubLog;
END ConsLog.
