type json_error_response = Defs_bs.base_error_response_t(Js.Json.t);

let read_json_error_response: Atdgen_codec_decode.t(json_error_response) =
  Defs_bs.read_base_error_response_t(Fun.id);

let write_json_error_response: Atdgen_codec_encode.t(json_error_response) =
  Defs_bs.write_base_error_response_t(Fun.id);
