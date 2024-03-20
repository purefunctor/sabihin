module ByteOctets = struct
  let t =
    Caqti_type.(
      custom
        ~encode:(fun v -> Ok (Bytes.to_string v))
        ~decode:(fun v -> Ok (Bytes.of_string v))
        octets)
end
