type yojson_error_response = Yojson.Safe.t Defs_t.base_error_content_t

let string_of_unparsed_error_content =
  Defs_j.string_of_base_error_content_t Buffer.add_string

let string_of_unparsed_error_response =
  Defs_j.string_of_base_error_response_t Buffer.add_string

let yojson_error_response_of_string =
  Defs_j.base_error_response_t_of_string Yojson.Safe.read_t

let string_of_yojson_error_response =
  Defs_j.string_of_base_error_response_t Yojson.Safe.write_t
