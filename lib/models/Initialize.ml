let initialize db =
  let ( let* ) = Lwt_result.bind in
  let* _ = User.create_table db in
  let* _ = PrivateKeys.create_table db in
  let* _ = PublicKeys.create_table db in
  Lwt.return_ok ()
