webpackHotUpdate("main",{

/***/ "./src/components/UserForm.jsx":
/*!*************************************!*\
  !*** ./src/components/UserForm.jsx ***!
  \*************************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* WEBPACK VAR INJECTION */(function(__react_refresh_utils__, __react_refresh_error_overlay__) {/* harmony import */ var react__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! react */ "./node_modules/react/index.js");
/* harmony import */ var react__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(react__WEBPACK_IMPORTED_MODULE_0__);
/* harmony import */ var _renderQuestions_RenderElements__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ../renderQuestions/RenderElements */ "./src/renderQuestions/RenderElements.jsx");
/* harmony import */ var _renderQuestions_FormContext__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! ../renderQuestions/FormContext */ "./src/renderQuestions/FormContext.jsx");
/* harmony import */ var _components_Confirm__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ../components/Confirm */ "./src/components/Confirm.jsx");
/* harmony import */ var axios__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! axios */ "./node_modules/axios/index.js");
/* harmony import */ var axios__WEBPACK_IMPORTED_MODULE_4___default = /*#__PURE__*/__webpack_require__.n(axios__WEBPACK_IMPORTED_MODULE_4__);
/* harmony import */ var _material_ui_core__WEBPACK_IMPORTED_MODULE_5__ = __webpack_require__(/*! @material-ui/core */ "./node_modules/@material-ui/core/index.js");
/* harmony import */ var react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__ = __webpack_require__(/*! react/jsx-dev-runtime */ "./node_modules/react/jsx-dev-runtime.js");
/* harmony import */ var react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6___default = /*#__PURE__*/__webpack_require__.n(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__);
__webpack_require__.$Refresh$.runtime = __webpack_require__(/*! ./node_modules/react-refresh/runtime.js */ "./node_modules/react-refresh/runtime.js");
__webpack_require__.$Refresh$.setup(module.i);

var _jsxFileName = "/home/hiroshi/projects/be-upp-frontend/frontend/src/components/UserForm.jsx",
    _s = __webpack_require__.$Refresh$.signature();









const UserForm = () => {
  _s();

  const [allElements, setAllElements] = Object(react__WEBPACK_IMPORTED_MODULE_0__["useState"])();
  const [steps, setSteps] = Object(react__WEBPACK_IMPORTED_MODULE_0__["useState"])(4);
  const [answers] = Object(react__WEBPACK_IMPORTED_MODULE_0__["useState"])({});
  const [isLoading, setLoading] = Object(react__WEBPACK_IMPORTED_MODULE_0__["useState"])(true);
  const [errorQuestions, setErrorQuestions] = Object(react__WEBPACK_IMPORTED_MODULE_0__["useState"])({});
  Object(react__WEBPACK_IMPORTED_MODULE_0__["useEffect"])(() => {
    axios__WEBPACK_IMPORTED_MODULE_4___default.a.get(`http://localhost:3001/open-api/template/latest`).then(response => {
      setAllElements(response.data);
      setLoading(false);
    });
  }, []);

  const checkAdvance = () => {
    const allPageQuestions = allElements.pages[steps].questions;
    const notAnswered = {};
    let allGood = true;
    Object.entries(allPageQuestions).map(([questionID, questionInfo]) => {
      if (questionInfo.type !== "pureText" && questionInfo.type !== "image" && questionInfo.type !== "scale" && questionInfo.type !== "table" && (!answers.hasOwnProperty(questionID) || !answers[questionID].value || Object.keys(answers[questionID].value).length === 0)) {
        notAnswered[questionID] = {
          value: true,
          errorText: "Favor preencher."
        };
        allGood = false;
      } // table checking separately


      if (questionInfo.type === "table") {
        if (!answers.hasOwnProperty(questionID) || Object.keys(answers[questionID].value).length !== Object.keys(allPageQuestions[questionID].row).length) {
          notAnswered[questionID] = {
            value: true,
            errorText: "Favor preencher."
          };
          allGood = false;
        }
      }
    });
    setErrorQuestions(notAnswered);
    return allGood;
  };

  const nextStep = () => {
    if (checkAdvance()) {
      window.scrollTo({
        top: 0,
        behavior: "auto"
      });
      setSteps(steps + 1);
    }
  };

  const prevStep = () => {
    window.scrollTo({
      top: 0,
      behavior: "auto"
    });
    setSteps(steps - 1);
  };

  const handleChange = (questionId, answer) => answers[questionId] = answer;

  const sendData = () => {
    const questions = [];
    Object.entries(answers).map(([questionID, questionInfo]) => questions.push({
      id: questionID,
      value: questionInfo.value
    }));
    const preparedData = {
      questions: questions,
      templateVersion: allElements.templateVersion,
      doctorId: 1
    };
    axios__WEBPACK_IMPORTED_MODULE_4___default.a.post(`http://localhost:3001/open-api/form-data/`, preparedData).then(response => {
      alert("Foi enviado. Parabéns!");
    }).catch(error => {
      alert("Ocorreu um erro no POST Template!");
    });
  };

  if (!isLoading) {
    var _allElements$pages$st;

    const {
      questions,
      pageLabel
    } = (_allElements$pages$st = allElements.pages[steps]) !== null && _allElements$pages$st !== void 0 ? _allElements$pages$st : {};
    const nPages = allElements.pages.length;
    var dict = {};
    Object.keys(allElements["pages"]).forEach(function (key) {
      //questions
      Object.keys(allElements["pages"][key]["questions"]).forEach(function (key2) {
        //type
        dict[key2] = [allElements["pages"][key]["questions"][key2]["type"], allElements["pages"][key]["questions"][key2]["questionLabel"]];
      });
    });
    if (!(steps === nPages)) return /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_renderQuestions_FormContext__WEBPACK_IMPORTED_MODULE_2__["FormContext"].Provider, {
      value: {
        handleChange
      },
      children: [/*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_material_ui_core__WEBPACK_IMPORTED_MODULE_5__["AppBar"], {
        style: {
          marginBottom: 20
        },
        position: "sticky",
        children: /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_material_ui_core__WEBPACK_IMPORTED_MODULE_5__["Typography"], {
          variant: "h4",
          component: "div",
          sx: {
            flexGrow: 1
          },
          children: pageLabel
        }, void 0, false, {
          fileName: _jsxFileName,
          lineNumber: 117,
          columnNumber: 13
        }, undefined)
      }, void 0, false, {
        fileName: _jsxFileName,
        lineNumber: 116,
        columnNumber: 11
      }, undefined), /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])("form", {
        children: [questions ? Object.entries(questions).map(([questionId, questionInfo]) => /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_renderQuestions_RenderElements__WEBPACK_IMPORTED_MODULE_1__["default"], {
          props: {
            questionId: questionId,
            answers: answers,
            error: errorQuestions[questionId],
            answer: answers[questionId],
            ...questionInfo
          }
        }, questionId, false, {
          fileName: _jsxFileName,
          lineNumber: 129,
          columnNumber: 17
        }, undefined)) : null, /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])("br", {}, void 0, false, {
          fileName: _jsxFileName,
          lineNumber: 139,
          columnNumber: 13
        }, undefined)]
      }, void 0, true, {
        fileName: _jsxFileName,
        lineNumber: 126,
        columnNumber: 11
      }, undefined), steps > 0 && /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_material_ui_core__WEBPACK_IMPORTED_MODULE_5__["Button"], {
        color: "secondary",
        variant: "contained",
        style: styles.buttonBack,
        onClick: () => prevStep(),
        children: " Voltar "
      }, void 0, false, {
        fileName: _jsxFileName,
        lineNumber: 143,
        columnNumber: 13
      }, undefined), steps < nPages && /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_material_ui_core__WEBPACK_IMPORTED_MODULE_5__["Button"], {
        color: "primary",
        variant: "contained",
        style: styles.buttonContinue,
        onClick: () => nextStep(),
        children: " Continuar "
      }, void 0, false, {
        fileName: _jsxFileName,
        lineNumber: 152,
        columnNumber: 13
      }, undefined)]
    }, void 0, true, {
      fileName: _jsxFileName,
      lineNumber: 115,
      columnNumber: 9
    }, undefined);else {
      return /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(react__WEBPACK_IMPORTED_MODULE_0___default.a.Fragment, {
        children: [/*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_components_Confirm__WEBPACK_IMPORTED_MODULE_3__["default"], {
          dict: dict,
          answer: answers
        }, void 0, false, {
          fileName: _jsxFileName,
          lineNumber: 165,
          columnNumber: 11
        }, undefined), /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_material_ui_core__WEBPACK_IMPORTED_MODULE_5__["Button"], {
          color: "secondary",
          variant: "contained",
          style: styles.buttonBack,
          onClick: () => prevStep(),
          children: " Voltar "
        }, void 0, false, {
          fileName: _jsxFileName,
          lineNumber: 170,
          columnNumber: 11
        }, undefined), /*#__PURE__*/Object(react_jsx_dev_runtime__WEBPACK_IMPORTED_MODULE_6__["jsxDEV"])(_material_ui_core__WEBPACK_IMPORTED_MODULE_5__["Button"], {
          variant: "contained",
          style: styles.buttonSuccess,
          color: "primary",
          onClick: () => sendData(),
          children: " Submeter "
        }, void 0, false, {
          fileName: _jsxFileName,
          lineNumber: 177,
          columnNumber: 11
        }, undefined)]
      }, void 0, true, {
        fileName: _jsxFileName,
        lineNumber: 164,
        columnNumber: 9
      }, undefined);
    }
  }

  return null;
};

_s(UserForm, "CC3Dc1wyfidxFjpFU1aJSqGWbJk=");

_c = UserForm;
const styles = {
  buttonContinue: {
    marginLeft: 15,
    marginRight: 15,
    marginBottom: 15,
    backgroundColor: "#21b6ae"
  },
  buttonBack: {
    marginLeft: 15,
    marginRight: 15,
    marginBottom: 15,
    backgroundColor: '#999999',
    color: '#FFFFFF'
  },
  buttonSuccess: {
    marginLeft: 15,
    marginRight: 15,
    marginBottom: 15,
    backgroundColor: '#00C0FF',
    color: '#FFFFFF'
  }
};
/* harmony default export */ __webpack_exports__["default"] = (UserForm);

var _c;

__webpack_require__.$Refresh$.register(_c, "UserForm");

const currentExports = __react_refresh_utils__.getModuleExports(module.i);
__react_refresh_utils__.registerExportsForReactRefresh(currentExports, module.i);

if (true) {
  const isHotUpdate = !!module.hot.data;
  const prevExports = isHotUpdate ? module.hot.data.prevExports : null;

  if (__react_refresh_utils__.isReactRefreshBoundary(currentExports)) {
    module.hot.dispose(
      /**
       * A callback to performs a full refresh if React has unrecoverable errors,
       * and also caches the to-be-disposed module.
       * @param {*} data A hot module data object from Webpack HMR.
       * @returns {void}
       */
      function hotDisposeCallback(data) {
        // We have to mutate the data object to get data registered and cached
        data.prevExports = currentExports;
      }
    );
    module.hot.accept(
      /**
       * An error handler to allow self-recovering behaviours.
       * @param {Error} error An error occurred during evaluation of a module.
       * @returns {void}
       */
      function hotErrorHandler(error) {
        if (
          typeof __react_refresh_error_overlay__ !== 'undefined' &&
          __react_refresh_error_overlay__
        ) {
          __react_refresh_error_overlay__.handleRuntimeError(error);
        }

        if (typeof __react_refresh_test__ !== 'undefined' && __react_refresh_test__) {
          if (window.onHotAcceptError) {
            window.onHotAcceptError(error.message);
          }
        }

        __webpack_require__.c[module.i].hot.accept(hotErrorHandler);
      }
    );

    if (isHotUpdate) {
      if (
        __react_refresh_utils__.isReactRefreshBoundary(prevExports) &&
        __react_refresh_utils__.shouldInvalidateReactRefreshBoundary(prevExports, currentExports)
      ) {
        module.hot.invalidate();
      } else {
        __react_refresh_utils__.enqueueUpdate(
          /**
           * A function to dismiss the error overlay after performing React refresh.
           * @returns {void}
           */
          function updateCallback() {
            if (
              typeof __react_refresh_error_overlay__ !== 'undefined' &&
              __react_refresh_error_overlay__
            ) {
              __react_refresh_error_overlay__.clearRuntimeErrors();
            }
          }
        );
      }
    }
  } else {
    if (isHotUpdate && __react_refresh_utils__.isReactRefreshBoundary(prevExports)) {
      module.hot.invalidate();
    }
  }
}
/* WEBPACK VAR INJECTION */}.call(this, __webpack_require__(/*! ./node_modules/@pmmmwh/react-refresh-webpack-plugin/lib/runtime/RefreshUtils.js */ "./node_modules/@pmmmwh/react-refresh-webpack-plugin/lib/runtime/RefreshUtils.js"), __webpack_require__(/*! ./node_modules/react-dev-utils/refreshOverlayInterop.js */ "./node_modules/react-dev-utils/refreshOverlayInterop.js")))

/***/ })

})
//# sourceMappingURL=main.07ec486c9d7533ac8500.hot-update.js.map