type meta = {
  title: string,
  name: string,
  type_: string,
  placeholder: string,
};

module type FieldKind = {
  type kind;

  let meta: meta;
  let iconFn: unit => React.element;
  let hintFn: ValidationResult.t(kind) => React.element;
  let fieldCssFn: ValidationResult.t(kind) => string;
};

module MakeField = (FieldKind: FieldKind) => {
  let containerCss = [%cx
    {|
    & > label {
      display: block;
      margin-bottom: 0.5rem;
      margin-left: 1rem;
    }
    & > span {
      display: block;
      margin-top: 0.5rem;
      margin-left: 1rem;
    }
  |}
  ];

  let fieldCss = [%cx
    {|
    display: flex;
    align-items: center;

    box-sizing: border-box;
    padding: 1rem;
    background-color: $(Theme.backgroundSubtle);
    border-radius: 8px;
    border-width: 4px;
    border-style: solid;
    border-color: $(Theme.backgroundSubtle);
    gap: 0.5rem;

    &:focus-within {
      outline-width: 4px;
      outline-offset: 2px;
      outline-style: solid;
      outline-color: $(Theme.primary);
    }
  |}
  ];

  let inputCss = [%cx
    {|
    height: 100%;
    width: 100%;
    color: $(Theme.foreground);
    background-color: $(Theme.backgroundSubtle);
    border: 0;
    font-family: "Poppins";
    font-size: 1rem;
    line-height: 2rem;

    &:focus {
      border-radius: 0;
      outline: none;
    }

    &:placeholder-shown {
      text-overflow: ellipsis;
    }
  |}
  ];

  [@react.component]
  let make = (~hook: RegisterPageHooksCore.field_hook(FieldKind.kind)) => {
    let iconElement = FieldKind.iconFn();
    let hintElement = FieldKind.hintFn(hook.validation);
    let extraFieldCss = FieldKind.fieldCssFn(hook.validation);

    let autoComplete = "off";
    let {title, name, type_, placeholder} = FieldKind.meta;
    let id = name;

    <div className=containerCss>
      <label htmlFor=id> {React.string(title)} </label>
      <div className={fieldCss ++ extraFieldCss}>
        iconElement
        <input
          className=inputCss
          autoComplete
          id
          name
          type_
          placeholder
          value={hook.value}
          onChange={hook.onChange}
        />
      </div>
      hintElement
    </div>;
  };
};

let fieldSuccessCss = [%cx {| border-color: $(Theme.success) !important; |}];
let fieldWarningCss = [%cx {| border-color: $(Theme.success) !important; |}];
let fieldFailureCss = [%cx {| border-color: $(Theme.failure) !important; |}];

let successCss = [%cx {| color: $(Theme.success) |}];
let warningCss = [%cx {| color: $(Theme.warning) |}];
let failureCss = [%cx {| color: $(Theme.failure) |}];
