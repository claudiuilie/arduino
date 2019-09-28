// ultimele cursuri din MDP DB sunt necesare inainte de aporni testul
const currencyMDP = {
    'EUR': {
        'BNR_RATE': 4.7352,
        'FRM_BUY_RATE': 4.6856,
        'FRM_SELL_RATE': 4.8176,
        'INT_BUY_RATE': 4.6523,
        'INT_SELL_RATE': 4.7523,
    },
    'USD': {
        'BNR_RATE': 5.7352,
        'FRM_BUY_RATE': 5.6856,
        'FRM_SELL_RATE': 5.8176,
        'INT_BUY_RATE': 4.6523,
        'INT_SELL_RATE': 4.7523,
    },
    'GBP': {
        'BNR_RATE': 6.7352,
        'FRM_BUY_RATE': 6.6856,
        'FRM_SELL_RATE': 6.8176,
        'INT_BUY_RATE': 4.6523,
        'INT_SELL_RATE': 4.7523,
    },
    'RON': {
        'BNR_RATE': 1,
        'FRM_BUY_RATE': 1,
        'FRM_SELL_RATE': 1
    }
}


class flowSelector {

    constructor(segmentation, amount, curr) {
        this.counter = 1;
        // var url = `http://fx-dynamic-service-fx-integral.ocp-test.rbro.rbg.cc/v1/quote?`;
        this.url = 'http://www.mocky.io/v2/5d8fa7773200005600adecbc'
        this.currencyes = curr;
        this.quote;
        this.transactionAmount = amount;
        this.segmentation = segmentation;
        this.flow;
        this.report = {};
        this.testCaseName;
        this.fcy1;
        this.fcy2;
        this.reportTrigger = 0;
        // de mutat jsonul intro clasa cel mai probabil pt ca nu formulele se calculeaza cu datele din josnul primit din request
    }

    //se trimite cate un request pentru fiecare caz in parte  
    buildRequest() {
        if (this.counter < this.currencyes.length) {

            this.setQuote(this.currencyes[this.counter], this.currencyes[0], this.currencyes[0], 'BUY')
            this.setQuote(this.currencyes[this.counter], this.currencyes[0], this.currencyes[this.counter], 'BUY')
            this.setQuote(this.currencyes[0], this.currencyes[this.counter], this.currencyes[0], 'SELL')
            this.setQuote(this.currencyes[0], this.currencyes[this.counter], this.currencyes[this.counter], 'SELL')

            this.counter++;
            this.buildRequest()
        }
    }

    async setQuote(debitCurrency, creditCurrency, transactionCurrency, exchangeOperation) {
        // se construieste quote-ul pentru fiecare request
        this.quote =
            `treasurySegmentation=${this.segmentation} 
		&debitCurrency=${debitCurrency}
		&creditCurrency=${creditCurrency}
		&transactionAmount=${this.transactionAmount}
		&transactionCurrency=${transactionCurrency}
        &transactionType=FX`

        // console.log(this.quote)

        //se apeleaza dynamic service
        let getDynamicService = await fetch(this.url, { // +quote
            method: 'GET', // or 'POST'
            mode: 'same-origin',
            headers: {
                'Content-Type': 'application/json',
                'X-Request-Id': '1232-23424',
                'X-Trace-Id': 'test-trace-id',
                'cif': '03999383'
            }
        });

        let results = await getDynamicService.json();

        //1. determinare flux FX RON sau FX CROSS
        switch (true) {
            case debitCurrency == 'RON' || creditCurrency == 'RON':
                this.flow = 'FX_RON'
                break;
            case debitCurrency != 'RON' && creditCurrency != 'RON':
                this.flow = 'FX_CROSS'
                break;
        }

        // constructie structura raport 
        this.testCaseName = `${this.flow}: ${debitCurrency}->${creditCurrency}(${transactionCurrency}) ${exchangeOperation}`
        this.report[this.testCaseName] = {
            'quoteRequest': this.quote,
            'dynamicServiceResponse': JSON.stringify(results),
            'source': results.source, 'exchangeOperation': 't',
            'debitAmount': 0,
            'debitRate': 0,
            'creditAmount': 0,
            'creditRate': 0,
            'transactionAmount': 0,
            'transactionRate': 0,
            'test_case': `#test case: `,
            'status': 'passed'
        };
        this.report[this.testCaseName].test_case += this.testCaseSelector(debitCurrency, transactionCurrency, creditCurrency, exchangeOperation);

        // prima validare a raspunsului exchange operation req -> res
        if (results.exchangeOperation != exchangeOperation) {
            this.report[this.testCaseName].exchangeOperation = `Fail -> Sent: ${exchangeOperation} / Response: ${results.exchangeOperation}`;
            this.report[this.testCaseName].status = 'failed';
        }
        else if (results.exchangeOperation === exchangeOperation) {
            this.report[this.testCaseName].exchangeOperation = `Pass -> Sent: ${exchangeOperation} / Response: ${results.exchangeOperation}`;
        }

        //2. in functie de sursa raspunsului MDP/INTEGRAL trece prin validarile din spec
        switch (true) {
            case results.source == 'INTEGRAL' && this.flow == 'FX_RON':
            case results.source == 'INTEGRAL' && this.flow == 'FX_CROSS':
                this.integralValidator(debitCurrency, creditCurrency, exchangeOperation, transactionCurrency, this.flow, this.testCaseName, this.transactionAmount, results);
                break;

            case results.source == 'MDP' && this.flow == 'FX_RON':
            case results.source == 'MDP' && this.flow == 'FX_CROSS':
                this.mdpValidator(debitCurrency, creditCurrency, exchangeOperation, transactionCurrency, this.flow, this.testCaseName, this.transactionAmount, results);
                break;

            default:
                this.logError('NU exista sursa in raspuns.')
        }
    }

    //rulare test cases pentru sursa MDP
    mdpValidator(dc, cc, eo, tc, flow, testCaseName, transactionAmount, results) {
        let testCase = this.testCaseSelector(dc, tc, cc, eo);
        this.fcy1 = dc;
        this.fcy2 = cc;

        let testCaseSelector = new testCasesMap(results, currencyMDP[this.fcy1], currencyMDP[this.fcy2], currencyMDP.RON, transactionAmount);
        this.runTestCases(testCaseName, testCase, flow, testCaseSelector.testCasesMdp);
    }
    //rulare test cases pentru sursa INTEGRAL
    integralValidator(dc, cc, eo, tc, flow, testCaseName, transactionAmount, results) {
        let testCase = this.testCaseSelector(dc, tc, cc, eo);
        this.fcy1 = dc;
        this.fcy2 = cc;

        let testCaseSelector = new testCasesMap(results, currencyMDP[this.fcy1], currencyMDP[this.fcy2], currencyMDP.RON, transactionAmount);
        this.runTestCases(testCaseName, testCase, flow, testCaseSelector.testCasesIntegral);
    }
    //selectare si rulare tc-uri in functie de selectie (1,2,3,4)
    runTestCases(testCaseName, testCase, flow, testConditions) {

        for (var z in testConditions[flow][testCase]) {
            switch (true) {
                case testConditions[flow][testCase][z].expected !== testConditions[flow][testCase][z].result:
                    this.updateReport(testCaseName, z, testConditions[flow][testCase][z].expected, testConditions[flow][testCase][z].result, false)
                    break;
                case testConditions[flow][testCase][z].expected === testConditions[flow][testCase][z].result:
                    this.updateReport(testCaseName, z, testConditions[flow][testCase][z].expected, testConditions[flow][testCase][z].result, true)
                    break;
            }
        }

        console.log(`%c${testCaseName} -> ${this.report[testCaseName].status} `, 'color: ' + (this.report[testCaseName].status === 'failed' ? 'red' : 'green'))
        this.reportTrigger++;

        if (this.reportTrigger === (this.currencyes.length - 1) * 4) {
            this.generateReport(this.report);
            // console.table(this.report)
        }
    }
    //selectie test case in functie de flux, perechi valutare si sursa
    testCaseSelector(dc, tc, cc, eo) {

        switch (true) {
            case dc == tc && eo == 'BUY':
                return 1;
            case cc == tc && eo == 'BUY':
                return 2;
            case dc == tc && eo == 'SELL':
                return 3;
            case cc == tc && eo == 'SELL':
                return 4;
        }
    }
    //constructie raport testare, fupa fiecare test case
    updateReport(testCaseName, column, expected, result, status) {

        if (status === true) {
            this.report[testCaseName][column] = `Pass -> Expected: ${column}: ${expected} / Result: ${column}: ${result}`;

            if (this.report[testCaseName].status == 'passed') {
                this.report[testCaseName].status = 'passed';
            }
        }
        else {
            this.report[testCaseName][column] = `Fail -> Expected: ${column}: ${expected} / Result: ${column}: ${result}`;
            this.report[testCaseName].status = 'failed';
        }
    }
    //generare raport testare final
    generateReport(unit) {
        console.log('Tests results:')
        for (var r in unit) {
            var res = unit[r];
            console.groupCollapsed(`%c  %c${res.source}_${r}, ${res.test_case} -> ${res.status} `, 'background-color: ' + (res.status === 'failed' ? 'red' : 'green') + '; margin-right: 10px', 'background-color: transparent');
            console.table(unit[r]); // generare tabel cu rezultate
            console.groupEnd();
        }

    }

    logError(args) {
        console.log(`%c Fail: ${args}`, `background: #222; color: red`)
    }

}

class testCasesMap {

    constructor(response, fcy1Rates, fcy2Rates, ronRates, transactionAmount) {

        this.testCasesMdp = {
            'FX_RON':
            {
                1: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount * response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': 1,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                2: {
                    'debitAmount': {
                        'expected': transactionAmount / fcy1Rates.FRM_BUY_RATE,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': 1,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                3: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': 1,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy2Rates.FRM_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                4: {
                    'debitAmount': {
                        'expected': transactionAmount * fcy2Rates.FRM_SELL_RATE,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': 1,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy2Rates.FRM_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                }

            },
            'FX_CROSS':
            {
                1: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount * response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.FRM_SELL_RATE / ronRates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE / fcy2Rates.FRM_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                2: {
                    'debitAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.FRM_SELL_RATE / ronRates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE / fcy2Rates.FRM_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                3: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy2Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / ronRates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / fcy2Rates.FRM_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                4: {
                    'debitAmount': {
                        'expected': transactionAmount * response.transactionAmount.rate,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy2Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / ronRates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / fcy2Rates.FRM_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                }

            }
        }

        this.testCasesIntegral = {
            'FX_RON':
            {
                1: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.INT_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount * response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': 1,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.INT_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                2: {
                    'debitAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.INT_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': 1,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.INT_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                3: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': 1,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.INT_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy2Rates.INT_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                4: {
                    'debitAmount': {
                        'expected': transactionAmount * fcy2Rates.INT_SELL_RATE,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': 1,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.INT_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy2Rates.INT_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                }

            },
            'FX_CROSS':
            {
                1: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount * response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE / fcy1Rates.INT_BUY_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.INT_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                2: {
                    'debitAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy2Rates.FRM_BUY_RATE / ronRates.FRM_BUY_RATE / fcy1Rates.INT_BUY_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.INT_BUY_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                3: {
                    'debitAmount': {
                        'expected': transactionAmount,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / ronRates.FRM_BUY_RATE / fcy1Rates.INT_SELL_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount / response.transactionAmount.rate,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / ronRates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.INT_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                },
                4: {
                    'debitAmount': {
                        'expected': transactionAmount * fcy1Rates.INT_SELL_RATE,
                        'result': response.debitedAmount.amount
                    },
                    'debitRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / ronRates.FRM_BUY_RATE / fcy1Rates.INT_SELL_RATE,
                        'result': response.debitedAmount.rate
                    },
                    'creditAmount': {
                        'expected': transactionAmount,
                        'result': response.creditedAmount.amount
                    },
                    'creditRate': {
                        'expected': fcy1Rates.FRM_SELL_RATE / ronRates.FRM_SELL_RATE,
                        'result': response.creditedAmount.rate
                    },
                    'transactionAmount': {
                        'expected': transactionAmount,
                        'result': response.transactionAmount.amount
                    },
                    'transactionRate': {
                        'expected': fcy1Rates.INT_SELL_RATE,
                        'result': response.transactionAmount.rate
                    }
                }

            }
        }


    }

}

var dynamicService = new flowSelector('5RBRB', 250, ['RON', 'EUR', 'GBP', 'USD']); // se completeaza cu segmentarea,suma, si monedele, prima fiind moneda care se doreste a fi prima
dynamicService.buildRequest();



