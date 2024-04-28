open Extra

let initialize db =
  let ( let* ) = Lwt_result.bind in
  let* _ = Nanoid.initialize db in
  let* _ = User.create_table db in
  let* _ = Secrets.create_table db in
  Lwt.return_ok ()
