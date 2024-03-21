(** Integrates {{: https://github.com/viascom/nanoid-postgres/ } nanoid-postgres }.

{1 Links}

- {{: https://github.com/ai/nanoid } ai/nanoid }
- {{: https://github.com/viascom/nanoid-postgres/ } viascom/nanoid-postgres }

{1 License }

{v
Copyright 2023 Viascom Ltd liab. Co
 
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
v}

{1 Interface}

*)

val initialize :
  (module Caqti_lwt.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) Lwt_result.t
(** [initialize db] makes the [nanoid] function available in the database. *)
