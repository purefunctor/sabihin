[@react.component] [@mel.module "framer-motion"] [@mel.scope "motion"]
external span:
  (
    ~initial: Js.t({..})=?,
    ~animate: Js.t({..})=?,
    ~transition: Js.t({..})=?,
    ~className: string=?,
    ~children: React.element=?
  ) =>
  React.element =
  "span";
