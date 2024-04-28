module Base64Octets = struct
  let t =
    let encode v =
      Base64.decode v |> Result.map_error (fun (`Msg msg) -> msg)
    in
    let decode v =
      Base64.encode v |> Result.map_error (fun (`Msg msg) -> msg)
    in
    Caqti_type.(custom ~encode ~decode octets)
end
