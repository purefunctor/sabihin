open Vault_native

let it_works () =
  let salt_base =
    Salt.compute_base (`String "ABCDEF0123456789") |> Bytes.to_string
  in
  let salt_digest =
    Salt.compute_digest (`String "ABCDEF0123456789") |> Cstruct.to_hex_string
  in
  Alcotest.(check int) "length is 256" 256 (String.length salt_base);
  Alcotest.(check string)
    "salt is equal"
    "sabihin.phLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLABCDEF0123456789"
    salt_base;
  Alcotest.(check string)
    "digest is equal"
    "817a0d4828d8fd886f6443962c8d34b0ab44e7cc85cc377cd6676f491c35be34"
    salt_digest

let () =
  Alcotest.run "Vault"
    [ ("Salt", [ Alcotest.test_case "it works" `Quick it_works ]) ]
