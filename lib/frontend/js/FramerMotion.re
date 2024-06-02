[@react.component] [@mel.module "framer-motion"] [@mel.scope "motion"]
external div:
  (
    ~initial: 'initial=?,
    ~animate: 'animate=?,
    ~transition: 'transition=?,
    ~variants: 'variants=?,
    ~onClick: React.Event.Mouse.t => unit=?,
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
    ~variants: 'variants=?,
    ~onClick: React.Event.Mouse.t => unit=?,
    ~className: string=?,
    ~children: React.element=?
  ) =>
  React.element =
  "span";
