open Models
open Utils

let generate (module Db : Caqti_lwt.CONNECTION) =
  let inner =
    let open Caqti_request.Infix in
    let open Caqti_type.Std in
    (int ->! string) @@ "SELECT nanoid(? ::int)"
  in
  Db.find inner

let simple =
  let inner db =
    let (module Db : Rapper_helper.CONNECTION) = db in
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let length = 12 in
    let* id = generate db length in
    let _ = Alcotest.(check int) "length id = 12" (String.length id) length in
    Lwt.return_ok ()
  in
  make_test_case "simple" inner
