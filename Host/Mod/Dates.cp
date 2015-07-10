MODULE HostDates;

	(*
		A. V. Shiryaev, 2013.11

		Taken from original BlackBox 1.6 (for Windows) HostDialog
	*)

	IMPORT WinApi, Dates, Strings, HostRegistry;

	TYPE
		DatesHook = POINTER TO RECORD (Dates.Hook) END;

	(** time **)

	PROCEDURE (hook: DatesHook) GetTime (OUT d: Dates.Date; OUT t: Dates.Time);
		VAR dt: WinApi.SYSTEMTIME;
	BEGIN
		WinApi.GetLocalTime(dt);
		d.year := dt.wYear; d.month := dt.wMonth; d.day := dt.wDay;
		t.hour := dt.wHour; t.minute := dt.wMinute; t.second := dt.wSecond
	END GetTime;

	PROCEDURE (hook: DatesHook) GetUTCTime (OUT d: Dates.Date; OUT t: Dates.Time);
		VAR dt: WinApi.SYSTEMTIME;
	BEGIN
		WinApi.GetSystemTime(dt);
		d.year := dt.wYear; d.month := dt.wMonth; d.day := dt.wDay;
		t.hour := dt.wHour; t.minute := dt.wMinute; t.second := dt.wSecond
	END GetUTCTime;

	PROCEDURE (hook: DatesHook) GetUTCBias (OUT bias: INTEGER);
		VAR res: INTEGER; info: WinApi.TIME_ZONE_INFORMATION;
	BEGIN
		bias := 0;
		res := WinApi.GetTimeZoneInformation(info);
		IF res # -1 THEN
			IF BITS(res) = WinApi.TIME_ZONE_ID_DAYLIGHT THEN bias := info.Bias + info.DaylightBias
			ELSE bias := info.Bias + info.StandardBias
			END
		END
	END GetUTCBias;

	(* date / time *)

	PROCEDURE (hook: DatesHook) DateToString (d: Dates.Date; format: INTEGER; OUT str: ARRAY OF CHAR);
		VAR res, pos, i: INTEGER; time: WinApi.SYSTEMTIME; fmt: ARRAY 64 OF CHAR;
	BEGIN
		time.wYear := SHORT(d.year); time.wMonth := SHORT(d.month); time.wDay := SHORT(d.day);
		IF format = Dates.short THEN
			res := WinApi.GetDateFormatW(
				HostRegistry.localeId, WinApi.DATE_SHORTDATE, time, NIL, str, LEN(str))
		ELSIF format = Dates.long THEN
			res := WinApi.GetDateFormatW(HostRegistry.localeId, WinApi.DATE_LONGDATE, time, NIL, str, LEN(str))
		ELSE
			res := WinApi.GetLocaleInfoW(HostRegistry.localeId, WinApi.LOCALE_SLONGDATE, fmt, LEN(fmt));
			IF format # Dates.abbreviated THEN	(* remove weekday *)
				Strings.Find(fmt, "dddd", 0, pos); i := pos + 4;
				IF pos < 0 THEN Strings.Find(fmt, "ddd", 0, pos); i := pos + 3 END;
				IF pos >= 0 THEN
					WHILE (fmt[i] # 0X) & (CAP(fmt[i]) < "A") OR (CAP(fmt[i]) > "Z") DO INC(i) END;
					Strings.Replace(fmt, pos, i - pos, "")
				END
			END;
			IF format # Dates.plainLong THEN	(* abbreviated *)
				Strings.Find(fmt, "dddd", 0, pos);
				IF pos >= 0 THEN Strings.Replace(fmt, pos, 4, "ddd") END;
				Strings.Find(fmt, "MMMM", 0, pos);
				IF pos >= 0 THEN Strings.Replace(fmt, pos, 4, "MMM") END
			END;
			res := WinApi.GetDateFormatW(HostRegistry.localeId, {}, time, fmt, str, LEN(str))
		END;
		IF res = 0 THEN str := "?" END
	END DateToString;

	PROCEDURE (hook: DatesHook) TimeToString (t: Dates.Time; OUT str: ARRAY OF CHAR);
		VAR res: INTEGER; time: WinApi.SYSTEMTIME;
	BEGIN
		time.wHour := SHORT(t.hour); time.wMinute := SHORT(t.minute);
		time.wSecond := SHORT(t.second); time.wMilliseconds := 0;
		res := WinApi.GetTimeFormatW(HostRegistry.localeId, {}, time, NIL, str, LEN(str));
		IF res = 0 THEN str := "?" END
	END TimeToString;

	PROCEDURE Init;
		VAR datesHook: DatesHook;
	BEGIN
		NEW(datesHook); Dates.SetHook(datesHook)
	END Init;

BEGIN
	Init
END HostDates.
