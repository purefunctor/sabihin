let src = Logs.Src.create "sabihin.database"

include (val Logs.src_log src)

let initialize () =
  Fmt_tty.setup_std_outputs ();
  Logs.set_level (Some Logs.Info);
  Logs.set_reporter (Logs_fmt.reporter ())
