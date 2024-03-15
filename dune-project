(lang dune 3.14)

(using melange 0.1)

(name sabihin)

(generate_opam_files true)

(source
 (github purefunctor/sabihin))

(authors "Justin Garcia")

(maintainers "Justin Garcia")

(license AGPL-3.0-or-later)

(package
 (allow_empty)
 (name sabihin)
 (synopsis "Anonymous and Secure Confessions")
 (description "Anonymous and Secure Confessions.")
 (depends
  ocaml
  (dream
   (>= "1.0.0~alpha5"))
  (reason
   (>= "3.11.0"))
  (dune
   (>= "3.14"))
  (melange
   (>= "3.0.0"))
  (melange-fetch
   (>= "0.1.0"))
  (reason-react
   (>= "0.14.0"))
  (reason-react-ppx
   (>= "0.14.0"))
  (yojson
   (>= "2.1.2"))
  (ppx_yojson_conv
   (>= "v0.16.0")))
 (tags
  (fullstack web)))
