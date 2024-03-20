open Lwt.Syntax

let () =
  Lwt_main.run
  @@
  let* pool = Utils.connect_pool () in
  Alcotest_lwt.run "Models"
    [
      ( "User",
        [
          User.initialize pool;
          User.insert pool;
          User.insert_existing pool;
          User.get_by_id pool;
          User.get_by_username pool;
        ] );
      ( "PrivateKey",
        [
          PrivateKeys.initialize pool;
          PrivateKeys.insert pool;
          PrivateKeys.insert_existing pool;
          PrivateKeys.get_by_user_id pool;
          PrivateKeys.get_by_username pool;
        ] );
    ]
