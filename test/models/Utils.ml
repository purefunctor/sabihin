let make_database_url name =
  let credentials =
    let ( let* ) = Option.bind in
    let* username = Sys.getenv_opt "DATABASE_USERNAME" in
    let* password = Sys.getenv_opt "DATABASE_PASSWORD" in
    Some (username, password)
  in
  match credentials with
  | Some (username, password) ->
      Printf.sprintf "postgresql://%s:%s@localhost:5432/%s" username password
        name
  | None -> Printf.sprintf "postgresql://localhost:5432/%s" name

let default_db_url = make_database_url ""
let uri = Uri.of_string (make_database_url "sabihin_models_test")

let perish action =
  Lwt.bind action (function
    | Ok o -> Lwt.return o
    | Error e -> raise (Caqti_error.Exn e))

let connect_pool () =
  match Caqti_lwt.connect_pool ~max_size:5 uri with
  | Ok p -> Lwt.return p
  | Error e -> raise (Caqti_error.Exn e)

let initialize () =
  perish
  @@ Caqti_lwt.with_connection (Uri.of_string default_db_url) (fun connection ->
         let open Lwt_result.Infix in
         [%rapper execute "DROP DATABASE IF EXISTS sabihin_models_test"]
           () connection
         >>= fun _ ->
         [%rapper execute "CREATE DATABASE sabihin_models_test"] () connection)

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
