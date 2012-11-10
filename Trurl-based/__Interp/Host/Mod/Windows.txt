MODULE HostWindows;

	(* for Views *)

	IMPORT Windows, Controllers, Views, Files, Converters, Documents, Ports;

	TYPE
		Window = POINTER TO EXTENSIBLE RECORD (Windows.Window)
			next: Window;	(* window ring, to prevent garbage collection of windows *)
		END;

		Directory = POINTER TO EXTENSIBLE RECORD (Windows.Directory)
		END;

	VAR
		dir: Directory;
		winAnchor: Window;	(* list of all windows, from top to bottom, first is dumy header *)

	(** Window **)

	PROCEDURE (w: Window) ForwardCtrlMsg (VAR msg: Controllers.Message), EXTENSIBLE;
	BEGIN
		HALT(126)
	END ForwardCtrlMsg;

	PROCEDURE (w: Window) SetSize (width, height: INTEGER);
	BEGIN
		HALT(126)
	END SetSize;

	PROCEDURE (w: Window) SetTitle (title: Views.Title);
	BEGIN
		HALT(126)
	END SetTitle;

	PROCEDURE (w: Window) RefreshTitle;
	BEGIN
		HALT(126)
	END RefreshTitle;

	PROCEDURE (w: Window) GetTitle (OUT title: Views.Title);
	BEGIN
		HALT(126)
	END GetTitle;

	PROCEDURE (w: Window) SetSpec (loc: Files.Locator; name: Files.Name; conv: Converters.Converter);
	BEGIN
		HALT(126)
	END SetSpec;

	PROCEDURE (w: Window) MouseDown (x, y, time: INTEGER; modifiers: SET);
	BEGIN
		HALT(126)
	END MouseDown;

	PROCEDURE (w: Window) KeyDown (ch: CHAR; buttons: SET);
	BEGIN
		HALT(126)
	END KeyDown;

	PROCEDURE (w: Window) Close;
	BEGIN
		ASSERT(w.frame # NIL, 20);
		HALT(126);
		w.Close^;
		ASSERT(w.frame = NIL, 60)
	END Close;

	(* Directory *)

	PROCEDURE (d: Directory) Open (
		w: Windows.Window; doc: Documents.Document; flags: SET; name: Views.Title;
		loc: Files.Locator; fname: Files.Name; conv: Converters.Converter
	);
		VAR p: Ports.Port;
	BEGIN
		WITH w: Window DO
		END
	END Open;

	PROCEDURE (d: Directory) First (): Window;
	BEGIN
		RETURN winAnchor.next
	END First;

	PROCEDURE (d: Directory) Next (w: Windows.Window): Window;
	BEGIN
		IF w # NIL THEN RETURN w(Window).next ELSE RETURN NIL END
	END Next;

	PROCEDURE (d: Directory) New (): Window, EXTENSIBLE;
		VAR w: Window;
	BEGIN
		NEW(w); RETURN w
	END New;

	PROCEDURE (d: Directory) Focus (target: BOOLEAN): Window;
	BEGIN
		HALT(126);
		RETURN NIL
	END Focus;

	PROCEDURE (d: Directory) Select (w: Windows.Window; lazy: BOOLEAN);
	BEGIN
		WITH w: Window DO
			HALT(126)
		END
	END Select;

	PROCEDURE (d: Directory) GetThisWindow (p: Ports.Port; px, py: INTEGER;
																		OUT x, y: INTEGER; OUT w: Windows.Window);
	BEGIN
		w := NIL
	END GetThisWindow;

	PROCEDURE (d: Directory) Close (w: Windows.Window);
		VAR v, u: Windows.Window; h: Window;
	BEGIN
		h := winAnchor; WHILE (h.next # NIL) & (h.next # w) DO h := h.next END;
		IF h.next = w THEN
			IF ~w.sub THEN
				v := w.link;
				WHILE v # w DO u := v.link; v.Close; v := u END
			END;
			w.Close
		END
	END Close;

	PROCEDURE (d: Directory) GetBounds (OUT w, h: INTEGER);
	BEGIN
		HALT(126)
	END GetBounds;

	PROCEDURE Init;
		VAR d: Directory;
	BEGIN
		NEW(d); d.l := -1; d.t := -1; d.r := -1; d.b := -1; dir := d; Windows.SetDir(d);
		NEW(winAnchor); winAnchor.next := NIL;	(* dummy header *)
	END Init;

BEGIN
	Init
END HostWindows.
