open Cmdliner

let cmd =
  let info =
    let doc = "utilities for the database" in
    Cmd.info "database" ~doc
  in
  Cmd.group info [ Initialize.cmd ]

let () =
  Logging.initialize ();
  exit (Cmd.eval cmd)
