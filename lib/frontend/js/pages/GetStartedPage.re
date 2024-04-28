open GetStartedPageHooks;

[@react.component]
let make = () => {
  let (stage, setStage) = Stage.use();

  let onFinish = () => {
    setStage(_ => QuickActions);
    Js.Promise.resolve();
  };

  <GetStartedPageLayout>
    {switch (stage) {
     | Loading => <GetStartedPageInitial />
     | Generate => <GetStartedPageGenerate onFinish />
     | QuickActions => <GetStartedPageQuickActions />
     }}
  </GetStartedPageLayout>;
};
