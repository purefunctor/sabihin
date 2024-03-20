let uri = Uri.of_string "postgresql://localhost:5432/sabihin_tests"

let perish action =
  Lwt.bind action (function
    | Ok o -> Lwt.return o
    | Error e -> raise (Caqti_error.Exn e))

let connect_pool () =
  match Caqti_lwt.connect_pool ~max_size:5 uri with
  | Ok p -> Lwt.return p
  | Error e -> raise (Caqti_error.Exn e)

let make_test_case ?(speed : Alcotest.speed_level option) message action pool =
  let speed = Option.value speed ~default:`Quick in
  let test switch () =
    let transaction db =
      let open Lwt_result.Syntax in
      let (module Db : Caqti_lwt.CONNECTION) = db in

      Lwt_switch.add_hook (Some switch) (fun () -> perish @@ Db.rollback ());
      let* _ = Db.start () in
      action db
    in
    perish @@ Caqti_lwt.Pool.use transaction pool
  in
  Alcotest_lwt.test_case message speed test
