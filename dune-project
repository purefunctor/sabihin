(lang dune 3.14)

(using directory-targets 0.1)
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
  (atdgen
   (>= "2.15.0"))
  (caqti-driver-postgresql
   (>= "1.9.0"))
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
  (melange-atdgen-codec-runtime
   (>= "3.0.0"))
  (ppx_rapper
   (>= "3.1.0"))
  (ppx_rapper_lwt
   (>= "3.1.0"))
  (reason-react
   (>= "0.14.0"))
  (reason-react-ppx
   (>= "0.14.0"))
  server-reason-react
  styled-ppx
  (yojson
   (>= "2.1.2"))
  (ppx_yojson_conv
   (>= "v0.16.0"))
  (dotenv
   (>= "0.0.3"))
  (melange-jest
   (and :with-test
    (>= "0.1.1")))
  (alcotest 
   (and :with-test
    (>= 1.7.0)))
  (alcotest-lwt
   (and :with-test
    (>= 1.7.0)))
  (cohttp
   (and :with-test
    (>= 5.3.1)))
  (cohttp-lwt-unix
   (and :with-test
    (>= 5.3.0))))
 (tags
  (fullstack web)))
