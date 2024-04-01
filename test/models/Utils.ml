let options = Test_toolbox.Database.get_options_from_env ()
let default_db_uri = Test_toolbox.Database.make_url ~options ""
let test_db_name = "sabihin_models_test"

let perish action =
  Lwt.bind action (function
    | Ok o -> Lwt.return o
    | Error e -> raise (Caqti_error.Exn e))

let connect_pool () =
  let test_db_uri =
    Test_toolbox.Database.make_url ~options test_db_name |> Uri.of_string
  in
  match Caqti_lwt.connect_pool ~max_size:5 test_db_uri with
  | Ok p -> Lwt.return p
  | Error e -> raise (Caqti_error.Exn e)

let initialize () =
  perish
  @@ Caqti_lwt.with_connection (Uri.of_string default_db_uri) (fun connection ->
         Test_toolbox.Database.Q.initialize_database connection test_db_name)

let make_test_case ?(speed : Alcotest.speed_level option) message action pool =
  let speed = Option.value speed ~default:`Quick in
  let test _ () =
    let transaction db =
      let open Lwt_result.Syntax in
      let (module Db : Caqti_lwt.CONNECTION) = db in

      let* _ = Db.start () in
      Lwt.bind (action db) (function
        | Ok _ -> Db.rollback ()
        | Error e ->
            let* _ = Db.rollback () in
            raise (Caqti_error.Exn e))
    in
    perish @@ Caqti_lwt.Pool.use transaction pool
  in
  Alcotest_lwt.test_case message speed test
