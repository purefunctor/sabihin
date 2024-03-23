open HigherOrderHandlers

module Fields = struct
  open Ppx_yojson_conv_lib.Yojson_conv.Primitives

  type t = { username : string; auth_token : string } [@@deriving yojson]

  let from_string_opt value =
    try Some (value |> Yojson.Safe.from_string |> t_of_yojson) with _ -> None
end

let handler request =
  let inner Fields.{ username; auth_token } =
    Dream.respond @@ username ^ auth_token
  in
  with_json_body request Fields.from_string_opt inner
