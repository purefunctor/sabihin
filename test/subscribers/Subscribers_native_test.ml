let it_works () =
  let module S = Subscribers_native in
  let subscribers = S.create () in
  let first = ref false in
  let second = ref false in
  let third = ref false in
  let _ = S.add subscribers (fun () -> first := true) in
  let _ = S.add subscribers (fun () -> second := true) in
  let third_key = S.add subscribers (fun () -> third := true) in
  S.delete subscribers third_key;
  S.for_each subscribers (fun callback -> callback ());
  Alcotest.(check bool) "first is true" true !first;
  Alcotest.(check bool) "second is true" true !second;
  Alcotest.(check bool) "third is false" false !third;
  ()

let () =
  Alcotest.run "Subscribers"
    [ ("api", [ Alcotest.test_case "it works" `Quick it_works ]) ]
