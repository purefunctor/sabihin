[@react.component] [@mel.module "framer-motion"] [@mel.scope "motion"]
external div:
  (
    ~initial: 'initial=?,
    ~animate: 'animate=?,
    ~transition: 'transition=?,
    ~variants: 'variants=?,
    ~className: string=?,
    ~children: React.element=?
  ) =>
  React.element =
  "div";

[@react.component] [@mel.module "framer-motion"] [@mel.scope "motion"]
external span:
  (
    ~initial: 'initial=?,
    ~animate: 'animate=?,
    ~transition: 'transition=?,
    ~className: string=?,
    ~children: React.element=?
  ) =>
  React.element =
  "span";
