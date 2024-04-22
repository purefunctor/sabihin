type json_error_response = Definitions_bs.base_error_response(Js.Json.t);

let read_json_error_response: Atdgen_codec_decode.t(json_error_response) =
  Definitions_bs.read_base_error_response(Fun.id);

let write_json_error_response: Atdgen_codec_encode.t(json_error_response) =
  Definitions_bs.write_base_error_response(Fun.id);
