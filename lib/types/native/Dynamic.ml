open Types_universal

type yojson_error_response = Yojson.Safe.t Definitions_t.base_error_response

let string_of_unparsed_error_kind =
  Definitions_j.string_of_base_error_kind Buffer.add_string

let string_of_unparsed_error_response =
  Definitions_j.string_of_base_error_response Buffer.add_string

let yojson_error_response_of_string =
  Definitions_j.base_error_response_of_string Yojson.Safe.read_t

let string_of_yojson_error_response =
  Definitions_j.string_of_base_error_response Yojson.Safe.write_t
